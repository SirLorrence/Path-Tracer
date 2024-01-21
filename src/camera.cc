#include "../include/camera.h"

void Camera::Render(const RenderObject& world) {
  Initialize();
  std::vector<std::uint8_t> pixelArray{};
  std::chrono::high_resolution_clock::time_point startRenderTime = std::chrono::high_resolution_clock::now();
  std::vector<std::vector<Color>> pixelLocation(m_imgHeight, std::vector<Color>(m_imgWidth, Vec3::Zero()));

#ifdef ENABLE_MP
  std::clog << "Running OpenMP...\n";
  #pragma omp parallel for collapse(2)
#endif

#ifdef MTHREAD // this method is no good
  ThreadPool pool;
  for (uint32_t y = 0; y < m_imgHeight; ++y) {
    // logging the  progress
    std::clog << "\r Lines Remaining:" << (m_imgHeight - y) << ' ' << std::flush;
    pool.AssignThread([this, &pixelLocation, y, &world] {
      for (uint32_t x = 0; x < m_imgWidth; ++x) {
        Vec3 pixelColor = Vec3::Zero(); // white
        // get the aggregated average
        for (uint32_t sample = 0; sample < m_pixelSampleSize; ++sample) {
          Ray ray = GetRay(x, y);
          pixelColor += RayColor(ray, m_maxDepth, world);
        }
        pixelLocation[y][x] = WriteColor(pixelColor, m_pixelSampleSize);
      }
    });
  }
#elif MTHREAD_V2
  ThreadPool pool;
  uint32_t chunkSize{m_imgHeight / g_ThreadsAvailable};
  uint32_t start{};
  uint32_t end{};
  uint8_t padding{};

  for (uint32_t i = 0; i < g_ThreadsAvailable; i++) {
    start = end;
    padding = (i == 0) ? 1 : 0;
    end = start + chunkSize + padding;
    std::clog << "Thread " << i << ": S: " << start << " E: " << end << '\n';
    pool.AssignThread([this, &pixelLocation, &world, start, end] {
      for (uint32_t y = start; y < end; y++) {
        for (uint32_t x = 0; x < m_imgWidth; x++) {
          Vec3 pixelColor = Vec3::Zero();
          for (uint32_t sample = 0; sample < m_pixelSampleSize; sample++) {
            Ray ray = GetRay(x, y);
            pixelColor += RayColor(ray, m_maxDepth, world);
          }
          pixelLocation[y][x] = WriteColor(pixelColor, m_pixelSampleSize);
        }
      }
    });
  }

  pool.CleanThreads();
#elif MTHREAD_TILE
  ThreadPool pool;
  if (g_ThreadsAvailable < 2) {
    std::cerr << "Not enough available cores/threads \n";
    std::exit(EXIT_FAILURE);
  }
  // sqrt is exspensive but got the job done
  uint32_t chunkX = m_imgWidth / g_ThreadsAvailable;
  uint32_t chunkY = m_imgHeight / g_ThreadsAvailable;
  std::clog << "cx: " << chunkX << " cy:" << chunkY << '\n';
  for (uint32_t y = 0; y < m_imgHeight; y += chunkY) {
    std::clog << "\r Lines Remaining:" << (m_imgHeight - y) << ' ' << std::flush;
    for (uint32_t x = 0; x < m_imgWidth; x += chunkX) {
      pool.AssignThread([this,&pixelLocation,x,y,&world,&chunkX,&chunkY] {
        for (uint32_t y1 = y; y1 < y + chunkY; ++y1) {
          if (y1 >= m_imgHeight) break;
          for (uint32_t x1 = x; x1 < x + chunkX; ++x1) {
            if (x1 >= m_imgWidth) break;
            Vec3 pixelColor = Vec3::Zero();
            for (uint32_t sample = 0; sample < m_pixelSampleSize; sample++) {
              Ray ray = GetRay(x1, y1);
              pixelColor += RayColor(ray, m_maxDepth, world);
            }
            pixelLocation[y1][x1] = WriteColor(pixelColor, m_pixelSampleSize);
          }
        }
      });
    }
  }
  pool.CleanThreads();
#else  // Single Threaded
  for (uint32_t y = 0; y < m_imgHeight; ++y) {
    // logging the  progress
#if !ENABLE_MP && !ENABLE_ACC
    std::clog << "\r Lines Remaining:" << (m_imgHeight - y) << ' ' << std::flush;
#endif
    for (uint32_t x = 0; x < m_imgWidth; ++x) {
      Vec3 pixelColor = Vec3::Zero(); // white
      // get the aggregated average
      for (uint32_t sample = 0; sample < m_pixelSampleSize; ++sample) {
        Ray ray = GetRay(x, y);
        pixelColor += RayColor(ray, m_maxDepth, world);
      }
      pixelLocation[y][x] = WriteColor(pixelColor, m_pixelSampleSize);
    }
  }

#endif  // MTHREAD & MTHREAD_V2

  for (uint32_t y = 0; y < m_imgHeight; y++) {
    for (uint32_t x = 0; x < m_imgWidth; x++) {
      pixelArray.push_back(static_cast<uint8_t>(pixelLocation[y][x].x()));
      pixelArray.push_back(static_cast<uint8_t>(pixelLocation[y][x].y()));
      pixelArray.push_back(static_cast<uint8_t>(pixelLocation[y][x].z()));
    }
  }
  std::chrono::high_resolution_clock::time_point stopRenderTime = std::chrono::high_resolution_clock::now();
  // extra white space to overwrite any logs
  std::clog << "\r Render Done.                    \n";
  std::chrono::milliseconds render_time_ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(stopRenderTime - startRenderTime);
  std::clog << "Render Performed in: " << render_time_ms.count() << "ms\n";
  std::clog << "PNG Created\n";
  std::string image_name{"../images/Ray Render " + GetTimeStamp() + ".png"};
  std::ofstream f_out{image_name, std::ios::binary};
  TinyPngOut png_image{
    static_cast<std::uint32_t>(m_imgWidth),
    static_cast<std::uint32_t>(m_imgHeight), f_out
  };
  // data() creates a pointer to the array not to the type of vector
  png_image.write(pixelArray.data(), static_cast<std::size_t>(m_imgWidth * m_imgHeight));
}

void Camera::Initialize() {
  m_imgHeight = static_cast<int>(m_imgWidth / m_aspectRatio);
  m_imgHeight = (m_imgHeight < 1) ? 1 : m_imgHeight;
  std::clog << "W: " << m_imgWidth << " H: " << m_imgHeight << "\n";
  m_center = m_lookFrom;

  // viewport dimensions
  // double focal_length = (look_from - look_at).Length();
  double theta = DegreesToRadians(m_viewFOV);
  double h = std::tan(theta / 2);
  double viewpointHeight = 2 * h * m_focusDistance;
  double viewpointWidth =
    viewpointHeight * (static_cast<double>(m_imgWidth) / m_imgHeight);

  // calculate basis vectors for camera frame
  m_w = Normalized(m_lookFrom - m_lookAt);
  m_u = Normalized(CrossProduct(m_viewUp, m_w));
  m_v = CrossProduct(m_w, m_u);

  Vec3 viewportX = viewpointWidth * m_u;
  Vec3 viewportY = viewpointHeight * -m_v;

  m_pixelDeltaX = viewportX / m_imgWidth;
  m_pixelDeltaY = viewportY / m_imgHeight;

  Vec3 viewportUpperLeft = m_center - (m_focusDistance * m_w) - viewportX / 2 - viewportY / 2;
  m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaX + m_pixelDeltaY);

  // defocus disk
  double defocusRadius = m_focusDistance * std::tan(DegreesToRadians(m_defocusAngle / 2));
  m_DefocusDiskX = m_u * defocusRadius;
  m_defocusDiskY = m_v * defocusRadius;
}

Color Camera::RayColor(const Ray& ray, int depth, const RenderObject& world) const {
  HitRecord hitRecord;

  // no more light is gathered.
  if (depth <= 0) return Color(0, 0, 0);

  if (world.Hit(ray, Interval(0.001, kInfinity), hitRecord)) {
    Ray scatteredRay;
    Color attenuation;
    if (hitRecord.m_material->Scatter(ray, hitRecord, attenuation, scatteredRay))
      return attenuation * RayColor(scatteredRay, depth - 1, world);
    return Color(0, 0, 0);

    // Diffused
    //    Vec3 direction = RandomOnHemisphere(hit_record.normal);
    // Limbertian
    //  Vec3 direction = hit_record.normal + RandomNormalizedVec3();s
    //    return 0.5 * RayColor(Ray(hit_record.point, direction), depth - 1,
    //    world);
  }

  Vec3 unitDirection = Normalized(ray.Direction());
  double blendedValue = 0.5 * (unitDirection.y() + 1.0);
  return (1.0 - blendedValue) * Color(1.0, 1.0, 1.0) +
    blendedValue * Color(0.5, 0.7, 1.0);
}

Ray Camera::GetRay(int coordinateX, int coordinateY) {
  Vec3 pixelCenter = m_pixel00Loc + (coordinateX * m_pixelDeltaX) + (coordinateY * m_pixelDeltaY);
  Vec3 pixelSample = pixelCenter + PixelSampleSquare();
  Vec3 origin = (m_defocusAngle <= 0) ? m_center : DefocusDiskSample();
  Vec3 direction = pixelSample - origin;
  return Ray(origin, direction);
}

Vec3 Camera::DefocusDiskSample() const {
  Vec3 v = RandomInDisk();
  return m_center + (v[0] * m_DefocusDiskX) + (v[1] * m_defocusDiskY);
}

// Return an random point in the surrounding square of the pixel origin.
Vec3 Camera::PixelSampleSquare() {
  double randomPointX = -0.5 + RandomDouble01();
  double randomPointY = -0.5 + RandomDouble01();
  return (randomPointX * m_pixelDeltaX) + (randomPointY * m_pixelDeltaY);
}

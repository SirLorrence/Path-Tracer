#pragma once
#include "global.h"

#include "color.h"
#include "render_object.h"
#include "material.h"

#include <string>
#include <fstream>
#include <iostream>

#include "../lib/TinyPngOut.h"

class Camera {
public:
  void Render(const RenderObject &world);
  Color RayColor(const Ray &ray, int depth, const RenderObject &world) const;

  Vec3 m_lookFrom = Vec3(0,0,0); // z = -1 breaks it - creating a blank render
  Vec3 m_lookAt = Vec3(0,0,-1);
  Vec3 m_viewUp = Vec3(0,1,0);

  double m_aspectRatio = 1.0;  // width/height
  double m_viewFOV = 90;
  double m_defocusAngle = 0; // variation angle of rays through each pixel
  double m_focusDistance = 10;

  int32_t m_imgWidth = 100;        // width in pixels
  int32_t m_pixelSampleSize = 10; // amount of random sample per pixel (AA)
  int32_t m_maxDepth = 10;         // recursion depth of ray bounces

private:
  void Initialize();
  Ray GetRay(int coordinateX, int coordinateY);
  Vec3 DefocusDiskSample() const;
  Vec3 PixelSampleSquare();

  Vec3 m_center;
  Vec3 m_pixel00Loc;
  // pixel spacing
  Vec3 m_pixelDeltaX;
  Vec3 m_pixelDeltaY;
  // Camera frame basis (i dont like these names)
  Vec3 m_u;
  Vec3 m_v;
  Vec3 m_w;
  // focus radius
  Vec3 m_DefocusDiskX;
  Vec3 m_defocusDiskY;

  int32_t m_imgHeight{};

};


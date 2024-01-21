#include "../include/global.h"

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable-list.h"
#include "../include/material.h"
#include "../include/sphere.h"

int main() {
  // World Objects
  HittableList world;
  std::shared_ptr<Material> materialGround = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.Add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, materialGround));


  constexpr int renderScatteredObjectsAmount = 11;
  for (int i = -renderScatteredObjectsAmount; i < renderScatteredObjectsAmount; i++) {
    for (int j = -renderScatteredObjectsAmount; j < renderScatteredObjectsAmount; j++) {
      const double chooseMaterial = RandomDouble01();
      Vec3 center(i + 0.9 * RandomDouble01(), 0.2, j + 0.9 * RandomDouble01());
      if ((center - Vec3(4, 0.2, 0)).Length() > 0.9) {
        std::shared_ptr<Material> sphere_material;

        if (chooseMaterial < 0.8) {
          Color albedo = Color::Random() * Color::Random();
          sphere_material = std::make_shared<Lambertian>(albedo);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
        else if (chooseMaterial < 0.95) {
          Color albedo = Color::Random(0.5, 1);
          double fuzz = RandomDouble(0.5, 1);
          sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
        else {
          sphere_material = std::make_shared<Dielectric>(1.5);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  std::shared_ptr<Material> material1 = std::make_shared<Dielectric>(1.5);
  world.Add(std::make_shared<Sphere>(Vec3(0,1,0),1.0, material1));
  std::shared_ptr<Material> material2 = std::make_shared<Lambertian>(Color(0.4,0.2,0.1));
  world.Add(std::make_shared<Sphere>(Vec3(-4,1,0),1.0, material2));
  std::shared_ptr<Material> material3 = std::make_shared<Metal>(Color(0.7,0.6,0.5),0.0);
  world.Add(std::make_shared<Sphere>(Vec3(4,1,0),1.0, material3));

  // Showcase Objects
  // std::shared_ptr<Material> sampleMaterial;
  // // sampleMaterial = std::make_shared<Lambertian>(Color::Random() * Color::Random());
  // // sampleMaterial = std::make_shared<Dielectric>((1.5));
  // sampleMaterial = std::make_shared<Metal>(Color(.5, .5, .5), 0.1);
  // world.Add(std::make_shared<Sphere>(Vec3(0, 1.25, -1), 1.0, sampleMaterial));
  // world.Add(std::make_shared<Sphere>(Vec3(0, 1.25, 1), 1.0, sampleMaterial));
  // world.Add(std::make_shared<Sphere>(Vec3(2, 1.25, -1), 1.0, sampleMaterial));
  // world.Add(std::make_shared<Sphere>(Vec3(2, 1.25, 1), 1.0, sampleMaterial));
  // world.Add(std::make_shared<Sphere>(Vec3(1, 2.25, 0), 1.0, sampleMaterial));


  Camera mainCamera;
  mainCamera.m_aspectRatio = 16.0 / 9.0;
  mainCamera.m_imgWidth = 600;
  mainCamera.m_pixelSampleSize = 100;
  mainCamera.m_maxDepth = 100;

  mainCamera.m_viewFOV = 20;
  mainCamera.m_lookFrom = Vec3(13,2,3);
  mainCamera.m_lookAt = Vec3(0,0,0);

  // Showcase Settings
  // mainCamera.m_viewFOV = 30;
  // mainCamera.m_lookFrom = Vec3(15, 5, -2.5);
  // mainCamera.m_lookAt = Vec3(0, 1, 0.25);

  mainCamera.m_defocusAngle = .6;

  mainCamera.Render(world);
}

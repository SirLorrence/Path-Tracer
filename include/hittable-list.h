#pragma once

#include "interval.h"
#include "render_object.h"

#include <memory>
#include <vector>

struct HittableList : public RenderObject {
  std::vector<std::shared_ptr<RenderObject>> render_objects;

  HittableList() {}
  HittableList(std::shared_ptr<RenderObject> object) { Add(object); }

  void Clear() { render_objects.clear(); }

  void Add(std::shared_ptr<RenderObject> obj) { render_objects.push_back(obj); }

  bool Hit(const Ray& ray, Interval rayScale, HitRecord& hitRecord) const override {
    HitRecord tempRecord;
    bool hitAnything = false;
    double closest = rayScale.m_max;

    for (const auto& obj : render_objects) {
      if (obj->Hit(ray, Interval(rayScale.m_min, closest), tempRecord)) {
        hitAnything = true;
        closest = tempRecord.m_scaler;
        hitRecord = tempRecord;
      }
    }
    return hitAnything;
  }
};
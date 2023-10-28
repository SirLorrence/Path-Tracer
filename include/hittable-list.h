#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

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

  bool Hit(const Ray &ray, Interval ray_t,
           HitRecord &hit_record) const override {
    HitRecord temp_record;
    bool hit_anything = false;
    double closest = ray_t.max;

    // TODO: make this an Object Type
    for (const auto &obj : render_objects) {
      if (obj->Hit(ray, Interval(ray_t.min, closest), temp_record)) {
        hit_anything = true;
        closest = temp_record.scaler;
        hit_record = temp_record;
      }
    }
    return hit_anything;
  }
};

#endif
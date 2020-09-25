//
// Created by samuel on 25/09/20.
//

#ifndef PHYVR_NDK_TANK_H
#define PHYVR_NDK_TANK_H

#include "entity.h"

#include <glm/glm.hpp>
#include <memory>

namespace phyvr_core {

    class Chassis : public Entity {
    public:
        std::shared_ptr<phyvr_view::Camera> get_cam();
    };

    class Wheel : public Entity {
    private:
        static constexpr float wheel_radius = 0.65f;
        static constexpr float wheel_width = 0.5f;

        static constexpr float wheel_offset = 0.5f;

        static constexpr float wheelbase_offset = 0.1f;

        static constexpr float wheel_mass = 100.f;

        static constexpr float wheel_color[4]{52.f / 255.f, 73.f / 255.f, 94.f / 255.f, 1.f};

    public:

        Wheel(AAssetManager *mgr, btDynamicsWorld *world,
              std::shared_ptr<phyvr_core::Chassis> *chassis,
              glm::vec3 chassis_pos,
              glm::vec3 pos);

        void update() override;

        void on_contact_finish(Entity *other) override;

        void damage(int to_sub) override;

    protected:
        btGeneric6DofSpring2Constraint *hinge;
    };

    class Turret : public Entity {

    };

    class Canon : public Entity {
    public:
        std::shared_ptr<phyvr_view::Camera> get_cam();
    };
}

#endif //PHYVR_NDK_TANK_H

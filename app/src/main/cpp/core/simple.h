//
// Created by samuel on 20/09/20.
//

#ifndef PHYVR_NDK_SIMPLE_H
#define PHYVR_NDK_SIMPLE_H

#include <glm/glm.hpp>

#include "entity.h"

namespace phyvr_core {
    class Cube : public Entity {
    public:
        Cube(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot_mat, float mass, int life);

        bool is_dead() override;

        bool explosion() override;

        void damage(int to_sub) override;

        void on_contact_finish(std::shared_ptr<Entity> other) override;

        void update() override;

    private:
        int life;
    };

    class Cone : public Entity {
    public:
        Cone(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot_mat, float mass, int life);

        bool is_dead() override;

        bool explosion() override;

        void on_contact_finish(std::shared_ptr<Entity> other) override;

        void damage(int to_sub) override;

        void update() override;

    private:
        int life;
    };
}

#endif //PHYVR_NDK_SIMPLE_H

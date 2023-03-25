//
// Created by samuel on 25/05/18.
//

#ifndef PHYVR_RENDERER_H
#define PHYVR_RENDERER_H

#include "../levels/level.h"
#include "glm/glm.hpp"
#include <android/asset_manager.h>
#include <vector>

class Renderer {
public:
  Renderer(Level *level);

  void update(glm::mat4 mHeadView, bool VR);

  void draw(glm::mat4 mEyeProjectionMatrix, glm::mat4 mEyeViewMatrix,
            glm::vec3 myLighPosInEyeSpace, glm::vec3 mCameraPos);

private:
  Level *level;
  glm::mat4 mCamera;
  glm::vec3 camPos;
  glm::vec3 camLookAtVec; // normalized Vec
  glm::vec3 camUpVec;
  bool VR;

  glm::vec3 updateLight(glm::mat4 viewMatrix, glm::vec3 xyz);
};

#endif // PHYVR_RENDERER_H

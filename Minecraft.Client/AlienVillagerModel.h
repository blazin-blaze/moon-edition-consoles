#pragma once
#include "Model.h"

class AlienVillagerModel : public Model
{
public:
    ModelPart* head, * body, * arms, * leg0, * leg1, * nose;

    void _init(float g, float yOffset, int xTexSize, int yTexSize); // 4J added
    AlienVillagerModel(float g, float yOffset, int xTexSize, int yTexSize);
    AlienVillagerModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim = 0);
};
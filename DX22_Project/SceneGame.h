#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#define MAX_TEST (8)

// includes
#include "Camera.h"
#include "Field.h"
#include "Scene.h"
#include "Effect.h"
// class
class CSceneGame : public CScene
{
public:
    CSceneGame();		
    ~CSceneGame();
    void Init() override;
    void Update() override;
    
};

#endif // __SCENE_GAME_H__
#include "Scene.h"

class CSceneGame : public CScene
{
public:
    CSceneGame();		
    ~CSceneGame();
    void Init() final;
    void Update() final;
    
};

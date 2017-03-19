#ifndef GAME_H
#define GAME_H

typedef void (*ACTION_COMPLETE) (void);

extern void DoSpin(ACTION_COMPLETE callback);
extern void DoForward(ACTION_COMPLETE callback);
extern void DoBackward(ACTION_COMPLETE callback);
extern void DoFlash(ACTION_COMPLETE callback);
extern void DoRainbow(ACTION_COMPLETE callback);
extern void DoSleep(ACTION_COMPLETE callback);
extern void DoBlink(ACTION_COMPLETE callback);
extern void DoCrossEye(ACTION_COMPLETE callback);
extern void DoCelebrate(ACTION_COMPLETE callback);
extern void DoShock(ACTION_COMPLETE callback);
extern void EndSpin(void);
extern void EndForward(void);
extern void EndBackward(void);
extern void EndFlash(void);
extern void EndRainbow(void);
extern void EndSleep(void);
extern void EndBlink(void);
extern void EndCrossEye(void);
extern void EndCelebrate(void);
extern void EndShock(void);

#endif

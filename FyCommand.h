/*========================================================================
  The Fly
  
  Command Management

  (C)2004-2010 Chuan-Chang Wang, All Rights Reserved
  Created : 0908, 2007 by Kevin C. Wang

  Last Updated : 1113, 2007
 =========================================================================*/
#if !defined(_THE_FLY_COMMAND_H_INCLUDED_)
#define _THE_FLY_COMMAND_H_INCLUDED_

#include "TheFlyWin32.h"


class FcCommandBase
{
private:
   BOOL m_bBeDone;                // Be done ?
   BOOL m_bCanBeUndo;             // Can be undo ?
   BOOL m_b1stDo;                 // be 1st do ?
   char m_aName[64];              // command name
   char m_aArgument[256];         // argument
   FcCommandBase *m_pNextCommand; // next command

public:
   FcCommandBase() { m_bBeDone = FALSE;
                     m_bCanBeUndo = TRUE;
                     m_b1stDo = TRUE;
                     m_aName[0] = '\0';
                     m_aArgument[0] = '\0';
                     m_pNextCommand = NULL; };

   virtual ~FcCommandBase() { };

   inline void NextCommand(FcCommandBase *cmd) { m_pNextCommand = cmd; };
   inline FcCommandBase *NextCommand() { return m_pNextCommand; };
   inline void BeDone(BOOL beDone) { m_bBeDone = beDone; };
   inline BOOL IsDone() { return m_bBeDone; };
   inline void BeUndo(BOOL beDone) { m_bCanBeUndo = beDone; };
   inline BOOL IsUndo() { return m_bCanBeUndo; };
   inline char *GetCommandName() { return m_aName; };
   inline void SetCommandName(char *name) { strcpy(m_aName, name); };
   inline void SetArgument(char *string) { strcpy(m_aArgument, string); };
   inline char *GetArgument() { return m_aArgument; };
   inline BOOL Is1stDo() { return m_b1stDo; };
   inline void Be1stDo(BOOL beDo) { m_b1stDo = beDo; };

   virtual void Init() { };
   virtual void DeInit() { };
   virtual BOOL Do(char *arg) { return FALSE; };
   virtual void Undo() { };
};


void FcSetupCommandQueue(int maxCmd);
void FcDeleteCommand(int iNum);
BOOL FcDoCommand(FcCommandBase *cmd, char *argument = NULL, BOOL beNoUndo = FALSE, BOOL beClearCmd = FALSE);
BOOL FcRedoCommand(char *cmdName, BOOL beTest = FALSE);
BOOL FcUndoLastCommand(char *cmdName, BOOL beTest = FALSE);
FcCommandBase *FcQueryCommand(int backPtr);
void FcClearCommand(void);

#endif _THE_FLY_COMMAND_H_INCLUDED_

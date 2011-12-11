/*========================================================================
  The Fly
  
  Command Script Management

  (C)2004-2010 Chuan-Chang Wang, All Rights Reserved
  Created : 0912, 2007 by Kevin C. Wang

  Last Updated : 1210, 2007
 =========================================================================*/
#include "FyCommand.h"
#include "FyMemSys.h"


// the command buffer
int fyCurCommandPtr = 0;
int fyMaxCommands = 0;
FcCommandBase **fyCommands = NULL;


/*-----------------------------
  initialize the command queue
  C.Wang 0912, 2007
 ------------------------------*/
void FcSetupCommandQueue(int num)
{
   int i;

   // clear out the existing command queue
   if (fyCommands != NULL) {
      for (i = 0; i < fyMaxCommands; i++) {
         if (fyCommands[i] != NULL) {
            delete fyCommands[i];
            fyCommands[i] = NULL;
         }
      }
      FyFree(fyCommands);
      fyCurCommandPtr = 0;
      fyMaxCommands = 0;
   }

   if (num > 0) {
      // create a new command queue
      fyMaxCommands = num;
      fyCommands = (FcCommandBase **)FyMalloc(sizeof(FcCommandBase *)*num);
      for (i = 0; i < fyMaxCommands; i++) {
         fyCommands[i] = NULL;
      }
      fyCurCommandPtr = 0;
   }
}


/*------------------------------------
  insert the command to command queue
  C.Wang 0513, 2009
 -------------------------------------*/
BOOL FcDoCommand(FcCommandBase *cmd, char *argument, BOOL beNoUndo, BOOL beClearCmd)
{
   int i;
   BOOL beOK = TRUE;

   if (argument != NULL) {
      cmd->SetArgument(argument);
   }

   // implement the command
   if (!cmd->IsDone()) {
       cmd->Be1stDo(TRUE);
       cmd->Init();
       beOK = cmd->Do(argument);
       if (!beOK) {
          cmd->DeInit();
          delete cmd;
       }
       else {
          cmd->BeDone(TRUE);
       }
   }
   if (beOK == FALSE) {
       return FALSE;
   }
   if (beClearCmd) {
       for (i = 0; i < fyMaxCommands; i++) {
           if (fyCommands[i] != NULL) {
               fyCommands[i]->DeInit();
               delete fyCommands[i];
               fyCommands[i] = NULL;
           }
       }
       fyCurCommandPtr = 0;
   }
   if (!beNoUndo) {
      if (cmd->IsUndo()) {
         // check the current command index in command queue
         // if there is space available
         if (fyCurCommandPtr <= fyMaxCommands - 1) {           
            // make the current command clear
            if (fyCommands[fyCurCommandPtr] != NULL) {
               fyCommands[fyCurCommandPtr]->DeInit();
               delete fyCommands[fyCurCommandPtr];
               fyCommands[fyCurCommandPtr] = NULL;
            }

            fyCommands[fyCurCommandPtr] = cmd;
            fyCurCommandPtr++;
         }
         else {
            // else the command queue is full
            // shift the commands to left and put the new command to the end of the queue
            FcCommandBase *toBeDelete = fyCommands[0];

            // delete the fyCommand[0]
            if (toBeDelete != NULL) {
               toBeDelete->DeInit();
               delete toBeDelete;
            }

            for (i = 1; i < fyMaxCommands; i++) {
               fyCommands[i - 1] = fyCommands[i];
            }
            fyCommands[fyMaxCommands - 1]  = cmd;
         }
      }
      else {
         // clean the undo command queue
         for (i = 0; i < fyMaxCommands; i++) {
            if (fyCommands[i] != NULL) {
               fyCommands[i]->DeInit();
               delete fyCommands[i];
               fyCommands[i] = NULL;
            }
         }
         fyCurCommandPtr = 0;
      }
   }


   // backup the command & argument
//   if (argument != NULL && strlen(argument) > 0) {
//      cmd->SetArgument(argument);
//   }

   return beOK;
}


/*----------------------
  undo the last command
  C.Wang 0922, 2007
 -----------------------*/
BOOL FcUndoLastCommand(char *log, BOOL beTest)
{
   if (fyCurCommandPtr <= 0) return FALSE;    // nothing in the queue
   if (beTest) return TRUE;

   FcCommandBase *cmd = fyCommands[fyCurCommandPtr - 1];
   fyCurCommandPtr--;

   // undo the last command
   if (cmd->IsDone()) {
      cmd->Undo();
      cmd->BeDone(FALSE);
   }

   if (log != NULL) {
      sprintf(log, "%s %s", cmd->GetCommandName(), cmd->GetArgument());
   }

   return TRUE;
}


/*------------------
  redo a command
  C.Wang 0912, 2007
 -------------------*/
BOOL FcRedoCommand(char *log, BOOL beTest)
{
   if (fyCurCommandPtr >= fyMaxCommands) return FALSE;
   if (fyCommands[fyCurCommandPtr] == NULL) return FALSE;
   if (beTest) return TRUE;

   FcCommandBase *cmd = fyCommands[fyCurCommandPtr];
   fyCurCommandPtr++;

   // redo the command
   if (!cmd->IsDone()) {
      cmd->Be1stDo(FALSE);
      cmd->Do(cmd->GetArgument());
      cmd->BeDone(TRUE);
   }

   if (log != NULL) {
      sprintf(log, "%s %s", cmd->GetCommandName(), cmd->GetArgument());
   }

   return TRUE;
}


/*------------------------
  query the last commands
  Brian Chang 1107, 2007
 -------------------------*/
FcCommandBase *FcQueryCommand(int backptr)
{
   int ptr = fyCurCommandPtr - backptr;
   if (ptr < 0) return NULL;
   FcCommandBase *cmd = fyCommands[ptr]; 
   return cmd;
}


/*-------------------------------------------------------
  delete the command in the command queue from it's back
  Brian Chang 1224, 2007
 --------------------------------------------------------*/
void FcDeleteCommand(int iNum)
{
   int i = 0;
   for (i = 0; i < iNum; i++) {
      fyCurCommandPtr -= 1;
      FcCommandBase *cmd = fyCommands[fyCurCommandPtr]; 
      if (cmd) {
         cmd->DeInit();
         delete cmd;
         fyCommands[fyCurCommandPtr] = NULL;
         cmd = NULL;
      } 
      else {
         return;
      }
   }
}


/*------------------------
  clear the command queue
  C.Wang 0624, 2010
 -------------------------*/
void FcClearCommand()
{
   int i;
   for (i = 0; i < fyMaxCommands; i++) {
      if (fyCommands[i] != NULL) {
         fyCommands[i]->DeInit();
         delete fyCommands[i];
         fyCommands[i] = NULL;
      }
   }
   fyCurCommandPtr = 0;
}
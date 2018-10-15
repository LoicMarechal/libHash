

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                               LIBHASH 1.30                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Description:       various operations on hash tables                     */
/*   Author:            Loic MARECHAL                                         */
/*   Creation date:     sep 25 2015                                           */
/*   Last modification: oct 15 2018                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "libhash.h"


/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/

#define TplSiz 8
#define MaxKey 4
#define UpdThr 2
#define IniBufSiz 10000

#ifndef MaxPth
#define MaxPth 128
#endif


/*----------------------------------------------------------------------------*/
/* Macros                                                                     */
/*----------------------------------------------------------------------------*/

#ifndef min
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifdef i8
#define lng int64_t
#else
#define lng int
#endif

#if defined(_NO_UNDER_SCORE)
#define call(x) x
#else
#define call(x) x ## _
#endif


/*----------------------------------------------------------------------------*/
/* Structures                                                                 */
/*----------------------------------------------------------------------------*/

typedef struct TplSctPtr
{
   char typ, cpt;
   int idx, tab[ TplSiz ];
   struct TplSctPtr *nxt;
}TplSct;

typedef struct
{
   char *typ[ MaxPth ];
   int IdxSiz, BlkSiz, NmbHsh, NmbTpl;
   int BufSiz[ MaxPth ][ MaxKey ], *buf[ MaxPth ][ MaxKey ];
   int64_t mem;
   TplSct **idx, *tpl, *IniBlk;
}HshTabSct;


/*----------------------------------------------------------------------------*/
/* Allocate a hash table with small initial sizes                             */ 
/*----------------------------------------------------------------------------*/

int64_t hsh_NewTable()
{
   int i, p;
   HshTabSct *tab;

   // Allocate the head table structure
   tab = calloc(1, sizeof(HshTabSct));
   assert(tab);
   tab->mem = sizeof(HshTabSct);

   // Allocate a local buffer for each key
   for(p=0;p<MaxPth;p++)
   {
      tab->typ[p] = malloc(IniBufSiz * sizeof(char));
      assert(tab->typ[p]);
   }

   for(p=0;p<MaxPth;p++)
      for(i=0;i<MaxKey;i++)
      {
         tab->BufSiz[p][i] = IniBufSiz;
         tab->buf[p][i] = malloc(tab->BufSiz[p][i] * sizeof(int));
         assert(tab->buf[p][i]);
         tab->mem += tab->BufSiz[p][i] * sizeof(int);
      }

   // Allocate the index table
   tab->IdxSiz = IniBufSiz;
   tab->idx = calloc(tab->IdxSiz, sizeof(void *));
   assert(tab->idx);
   tab->mem += tab->IdxSiz * sizeof(void *);

   // Allocate the first block of tupples
   tab->BlkSiz = IniBufSiz;
   tab->tpl = tab->IniBlk = calloc(tab->BlkSiz, sizeof(TplSct));
   assert(tab->tpl);
   tab->mem += tab->BlkSiz * sizeof(TplSct);

   // Return a pointer to the head structure casted into a int64_t int
   return((int64_t)tab);
}


/*----------------------------------------------------------------------------*/
/* Free the hash table and all linked tupple tables                           */
/*----------------------------------------------------------------------------*/

int hsh_FreeTable(int64_t HshIdx)
{
   HshTabSct *tab = (HshTabSct *)HshIdx;
   TplSct *nxt, *tpl = tab->IniBlk;
   int i, p, byt = 0;

   if(tab->NmbHsh > 0)
      byt = tab->mem / tab->NmbHsh;

   // Loop over blocks of tupple and free them.
   // The last tupple of each block contains nothing but a link to the next block
   do
   {
      nxt = tpl[ IniBufSiz-1 ].nxt;
      free(tpl);
      tab->mem -= tab->BlkSiz * sizeof(TplSct);
   }while((tpl = nxt));

   // Free the local search buffers of each key levels
   for(p=0;p<MaxPth;p++)
      for(i=0;i<MaxKey;i++)
      {
         free(tab->buf[p][i]);
         tab->mem -= tab->BufSiz[p][i] * sizeof(int);
      }

   // Free the main index table
   free(tab->idx);
   tab->mem -= tab->IdxSiz * sizeof(void *);

   // Free the types table
   for(p=0;p<MaxPth;p++)
      free(tab->typ[p]);
   //tab->mem -= tab->BufSiz[i] * sizeof(char);

   // And finaly, free the head structure. The tab->mem counter should be 0
   tab->mem -= sizeof(HshTabSct);
   free(tab);

   // Return the total number of allocated bytes per hashed items
   return(byt);
}


/*----------------------------------------------------------------------------*/
/* Add an element in the table according to its vertex index                  */
/*----------------------------------------------------------------------------*/

int hsh_AddItem(int64_t HshIdx, int typ, int VerIdx, int EleIdx, int ChkFlg)
{
   HshTabSct *tab = (HshTabSct *)HshIdx;
   int i, NewSiz, NewKey, key = VerIdx % tab->IdxSiz;
   TplSct *tpl = tab->idx[ key ], **NewTab, *nxt, *OldTpl, *EndTpl, *FreTpl=NULL;

   // Check whether this pair vertex/element is not present in the hash table
   if(ChkFlg)
   {
      while( tpl && !FreTpl )
      {
         if( (tpl->idx == VerIdx) && (tpl->typ == typ) )
         {
            for(i=0;i<tpl->cpt;i++)
               if(tpl->tab[i] == EleIdx)
                  return(0);

            if( !FreTpl && (tpl->cpt < TplSiz) )
               FreTpl = tpl;
         }

         tpl = tpl->nxt;
      }
   }
   else if(tpl && (tpl->typ == typ) && (tpl->cpt < TplSiz))
      FreTpl = tpl;

   // If not present and a partialy filled tupple was found, store it
   if(FreTpl)
   {
      FreTpl->tab[ (int)FreTpl->cpt++ ] = EleIdx;
      tab->NmbHsh++;
      return(1);
   }
   else
      tpl = NULL;

   // If no available tupple was found, get the next one from the tupple buffer
   if(!tpl)
   {
      tpl = &tab->tpl[ tab->NmbTpl++ ];

      // If the tupple buffer is exhausted, allocate a new one
      // and link it to the previous buffer last tupple
      if(tab->NmbTpl == tab->BlkSiz-1)
      {
         EndTpl = &tab->tpl[ tab->NmbTpl ];
         EndTpl->typ = -1;
         tab->tpl = EndTpl->nxt = calloc(tab->BlkSiz, sizeof(TplSct));
         assert(tab->tpl);
         tab->NmbTpl = 0;
         tab->mem += tab->BlkSiz * sizeof(TplSct);
      }

      // If the number of hashed items per index id greater than the threshold,
      // resize the index table. It can be done without rehashing
      // all the entries, only the index entries must be updated
      if(tab->NmbHsh > TplSiz * tab->IdxSiz * UpdThr)
      {
         NewSiz = tab->IdxSiz * UpdThr;
         NewTab = calloc(NewSiz, sizeof(void *));
         assert(NewTab);
         tab->mem += NewSiz * sizeof(void *);

         for(i=0;i<tab->IdxSiz;i++)
         {
            OldTpl = tab->idx[i];

            while(OldTpl)
            {
               nxt = OldTpl->nxt;
               NewKey = OldTpl->idx % NewSiz;
               OldTpl->nxt = NewTab[ NewKey ];
               NewTab[ NewKey ] = OldTpl;
               OldTpl = nxt;
            }
         }

         free(tab->idx);
         tab->mem -= tab->IdxSiz * sizeof(void *);
         tab->idx = NewTab;
         tab->IdxSiz = NewSiz;
      }
   }

   // When a new tupple is found, initialize it with this vertex, type and element
   tpl->idx = VerIdx;
   tpl->typ = typ;
   tpl->tab[ (int)tpl->cpt++ ] = EleIdx;
   tpl->nxt = tab->idx[ key ];
   tab->idx[ key ] = tpl;
   tab->NmbHsh++;

   return(1);
}


/*----------------------------------------------------------------------------*/
/* Remove a pair element-vertex from the table                                */
/*----------------------------------------------------------------------------*/

int hsh_DeleteItem(int64_t HshIdx, int typ, int VerIdx, int EleIdx)
{
   HshTabSct *tab = (HshTabSct *)HshIdx;
   int i, j, key = VerIdx % tab->IdxSiz;
   TplSct *tpl = tab->idx[ key ];

   while(tpl)
   {
      if( (tpl->idx == VerIdx) && (tpl->typ == typ) )
      {
         for(i=0;i<tpl->cpt;i++)
            if(tpl->tab[i] == EleIdx)
            {
               for(j=i+1;j<tpl->cpt;j++)
                  tpl->tab[j-1] = tpl->tab[j];

               tpl->cpt--;
               return(1);
            }
      }

      tpl = tpl->nxt;
   }

   return(0);
}


/*----------------------------------------------------------------------------*/
/* Find all entries that match the set of vertices                            */
/*----------------------------------------------------------------------------*/

int hsh_GetItem(  int64_t HshIdx, int PthIdx, int typ, int NmbVer,
                  int *VerTab, int **EleTab, char **TypTab )
{
   HshTabSct *tab = (HshTabSct *)HshIdx;
   int i, j, k, key, InsFlg, pos[ MaxKey ];
   TplSct *tpl;

   if( (NmbVer > MaxKey) || !VerTab || !EleTab )
      return(0);

   // For each key, look for the key's linked list for tupples
   // that matches it and that belongs to the previous list
   for(i=0;i<NmbVer;i++)
   {
      key = VerTab[i] % tab->IdxSiz;
      tpl = tab->idx[ key ];
      pos[i] = 0;

      while(tpl)
      {
         if( (tpl->idx == VerTab[i]) && ((tpl->typ == typ) || (typ == HshAny)) )
         {
            for(j=0;j<tpl->cpt;j++)
            {
               // If the first key is beeing searched, add any matching element
               // to the list, otherwise, look through the previous to check
               // whether it is present or not
               if(i)
               {
                  InsFlg = 0;

                  for(k=0;k<pos[i-1];k++)
                     if(tpl->tab[j] == tab->buf[ PthIdx ][i-1][k])
                     {
                        InsFlg = 1;
                        break;
                     }
               }
               else
                  InsFlg = 1;

               // If this item is to be added to the current list,
               // check for the key's table size. If we run out of space,
               // realloc the buffer with double the size
               if(InsFlg)
               {
                  tab->buf[ PthIdx ][i][ pos[i] ] = tpl->tab[j];

                  // Fill types table only if it is requested from the user
                  // and the last key is beeing parsed
                  if(TypTab && (i == NmbVer-1) )
                     tab->typ[ PthIdx ][ pos[i] ] = tpl->typ;

                  pos[i]++;

                  if(pos[i] == tab->BufSiz[ PthIdx ][i])
                  {
                     tab->buf[ PthIdx ][i] = realloc(tab->buf[ PthIdx ][i],
                        tab->BufSiz[ PthIdx ][i] * UpdThr * sizeof(int));
                     assert(tab->buf[ PthIdx ][i]);
                     tab->mem += tab->BufSiz[ PthIdx ][i] * (UpdThr - 1) * sizeof(int);

                     if(TypTab && (i == NmbVer-1) )
                     {
                        tab->typ[ PthIdx ] = realloc(tab->typ[ PthIdx ],
                           tab->BufSiz[ PthIdx ][i] * UpdThr * sizeof(char));
                        assert(tab->typ[ PthIdx ]);
                        tab->mem += tab->BufSiz[ PthIdx ][i] * (UpdThr - 1) * sizeof(char);
                     }

                     tab->BufSiz[ PthIdx ][i] *= UpdThr;
                  }
               }
            }
         }

         tpl = tpl->nxt;
      }
   }

   // Return last level tables into user's pointers
   *EleTab = tab->buf[ PthIdx ][ NmbVer-1 ];

   if(TypTab)
      *TypTab = tab->typ[ PthIdx ];

   // Return the number of elements in the last level table,
   // it is the intersection between every lists
   return(pos[ NmbVer-1 ]);
}


/*----------------------------------------------------------------------------*/
/* Fortran 77 API                                                             */
/*----------------------------------------------------------------------------*/

int64_t call(hshnewtable)()
{
   return(hsh_NewTable());
}

lng call(hshfreetable)(int64_t *HshIdx)
{
   return(hsh_FreeTable(*HshIdx));
}   

lng call(hshadditem)(int64_t *HshIdx, lng *typ, lng *VerIdx, lng *EleIdx, lng *ChkFlg)
{
   return(hsh_AddItem(*HshIdx, *typ, *VerIdx, *EleIdx, *ChkFlg));
}

lng call(hshdeleteitem)(int64_t *HshIdx, lng *typ, lng *VerIdx, lng *EleIdx)
{
   return(hsh_DeleteItem(*HshIdx, *typ, *VerIdx, *EleIdx));
}

lng call(hshgetitem)(int64_t *HshIdx, lng *PthIdx, lng *typ, lng *NmbVer, lng *VerTab,
                     lng *TabSiz, lng *EleTab, char *TypTab)
{
   char *TmpTypTab=NULL;
   int i;
   lng NmbEle, *TmpEleTab=NULL;

   NmbEle = hsh_GetItem(*HshIdx, *PthIdx, *typ, *NmbVer, VerTab, &TmpEleTab, &TmpTypTab);
   NmbEle = MIN(NmbEle, *TabSiz);

   if(TmpEleTab)
      for(i=0;i<NmbEle;i++)
         EleTab[i] = TmpEleTab[i];

   if(TmpTypTab)
      for(i=0;i<NmbEle;i++)
         TypTab[i] = TmpTypTab[i];

   return(NmbEle);
}

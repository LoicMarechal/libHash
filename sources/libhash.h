

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                               LIBHASH 1.21                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Description:       various operations on hash tables                     */
/*   Author:            Loic MARECHAL                                         */
/*   Creation date:     sep 25 2015                                           */
/*   Last modification: oct 23 2015                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/

enum HshTyp {HshAny, HshVer, HshEdg, HshTri, HshQad, HshTet, HshPyr, HshPri, HshHex};


/*----------------------------------------------------------------------------*/
/* Prototypes of public procedures                                            */
/*----------------------------------------------------------------------------*/

int64_t  hsh_NewTable   ();
int      hsh_FreeTable  (int64_t);
int      hsh_AddItem    (int64_t, int, int, int, int);
int      hsh_DeleteItem (int64_t, int, int, int);
int      hsh_GetItem    (int64_t, int, int, int *, int **, char **);

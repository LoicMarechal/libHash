

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                               LIBHASH 1.30                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Description:       various operations on hash tables                     */
/*   Author:            Loic MARECHAL                                         */
/*   Creation date:     sep 25 2015                                           */
/*   Last modification: mar 12 2018                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/

enum HshTyp {HshVer, HshEdg, HshTri, HshQad, HshTet, HshPyr, HshPri, HshHex, HshAny};


/*----------------------------------------------------------------------------*/
/* Prototypes of public procedures                                            */
/*----------------------------------------------------------------------------*/

int64_t  hsh_NewTable   ();
int      hsh_FreeTable  (int64_t);
int      hsh_AddItem    (int64_t, int, int, int, int);
int      hsh_DeleteItem (int64_t, int, int, int);
int      hsh_GetItem    (int64_t, int, int, int, int *, int **, char **);

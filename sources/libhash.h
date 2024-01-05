

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                               LIBHASH 1.41                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Description:       various operations on hash tables                     */
/*   Author:            Loic MARECHAL                                         */
/*   Creation date:     sep 25 2015                                           */
/*   Last modification: jan 04 2024                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#ifndef LIBHASH1_H
#define LIBHASH1_H


/*----------------------------------------------------------------------------*/
/* System includes                                                            */
/*----------------------------------------------------------------------------*/

#include <stdint.h>


/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/

enum HshTyp{HshVer, HshEdg, HshTri, HshQad,
            HshTet, HshPyr, HshPri, HshHex, HshAny};


/*----------------------------------------------------------------------------*/
/* Global defines                                                             */
/*----------------------------------------------------------------------------*/

#if defined(i8) || defined(INT64)
#define itg int64_t
#else
#define itg int32_t
#endif


/*----------------------------------------------------------------------------*/
/* Prototypes of public procedures                                            */
/*----------------------------------------------------------------------------*/

int64_t  hsh_NewTable   ();
itg      hsh_FreeTable  (int64_t);
itg      hsh_AddItem    (int64_t, itg, itg, itg, itg);
itg      hsh_DeleteItem (int64_t, itg, itg, itg);
itg      hsh_GetItem    (int64_t, itg, itg, itg, itg *, itg **, char **);

#endif

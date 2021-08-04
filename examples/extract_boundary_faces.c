

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*             FIND VERTEX BALLS, EDGE SHELLS AND NEIGHBOURS                  */
/*             IN A HYBRID MESH WITH THE HELP OF THE LIBHASH                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Description:       derive the surface from the volume                    */
/*   Author:            Loic MARECHAL                                         */
/*   Creation date:     sep 30 2015                                           */
/*   Last modification: aug 04 2021                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <libmeshb7.h>
#include <libhash.h>


/*----------------------------------------------------------------------------*/
/* Topologic tables                                                           */
/*----------------------------------------------------------------------------*/

int tettvpe[6][2] = { {0,1}, {1,2}, {2,0}, {3,0}, {3,1}, {3,2} };
int tettvpf[4][3] = { {1,2,3}, {2,0,3}, {3,0,1}, {0,2,1} };
int pyrtvpe[8][2] = { {0,1}, {3,2}, {0,3}, {1,2}, {0,4}, {1,4}, {2,4}, {3,4} };
int pyrtvpf[5][4] = { {0,1,4,-1}, {1,2,4,-1}, {2,3,4,-1}, {3,0,4,-1}, {3,2,1,0} };
int pritvpe[9][2] = { {0,1}, {1,2}, {2,0}, {3,4}, {4,5}, {5,3}, {0,3}, {1,4}, {2,5} };
int pritvpf[5][4] = { {3,5,2,0}, {1,2,5,4}, {0,1,4,3}, {2,1,0,-1}, {3,4,5,-1} };
int hextvpe[12][2] = {  {3,2}, {0,1}, {4,5}, {7,6}, {3,7}, {2,6},
                        {1,5}, {0,4}, {3,0}, {7,4}, {6,5}, {2,1} };
int hextvpf[6][4] = {   {3,0,4,7}, {6,5,1,2}, {3,2,1,0},
                        {4,5,6,7},{3,7,6,2}, {1,5,4,0} };


/*----------------------------------------------------------------------------*/
/* Mesh data structures                                                       */
/*----------------------------------------------------------------------------*/

typedef struct
{
   double crd[3];
   int ref;
}VerSct;

typedef struct
{
   int idx[2], ref;
}EdgSct;

typedef struct
{
   int idx[3], ref;
}TriSct;

typedef struct
{
   int idx[4], ref;
}QadSct;

typedef struct
{
   int idx[4], ref;
}TetSct;

typedef struct
{
   int idx[5], ref;
}PyrSct;

typedef struct
{
   int idx[6], ref;
}PriSct;

typedef struct
{
   int idx[8], ref;
}HexSct;


/*----------------------------------------------------------------------------*/
/* Extract the surface from a hybrid volume mesh                              */
/*----------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
   int i, j, k, MshVer, dim, NmbVer, NmbTri=0, NmbQad=0;
   int NmbPyr, NmbTet, NmbPri, NmbHex, *EleTab, VerFac[4];
   char *TypTab;
   int64_t MshIdx, HshIdx;
   VerSct *VerTab = NULL;
   TriSct *TriTab = NULL;
   QadSct *QadTab = NULL;
   TetSct *TetTab = NULL;
   PyrSct *PyrTab = NULL;
   PriSct *PriTab = NULL;
   HexSct *HexTab = NULL;

   // The program reads volume.meshb and writes surface.meshb files
   if(!(MshIdx = GmfOpenMesh("../sample_meshes/volume.meshb", GmfRead, &MshVer, &dim)))
   {
      puts("Cannot open file ../sample_meshes/volume.meshb");
      exit(1);
   }

   // Allocate and read vertices
   if(!(NmbVer = (int)GmfStatKwd(MshIdx, GmfVertices)))
   {
      puts("No vertices found in the mesh file");
      exit(1);
   }

   if(!(VerTab = malloc( (NmbVer+1) * sizeof(VerSct))))
      exit(1);

   GmfGetBlock(MshIdx, GmfVertices, 1, NmbVer, 0, NULL, NULL,
               GmfDoubleVec, 3,  VerTab[1].crd,  VerTab[ NmbVer ].crd,
               GmfInt,          &VerTab[1].ref, &VerTab[ NmbVer ].ref);

   if(!(HshIdx = hsh_NewTable()))
   {
      puts("Failed to allocate the hash table");
      exit(1);
   }

   printf("%d vertices read\n", NmbVer);


   // Check existence, allocate, read and hash the tets
   if((NmbTet = (int)GmfStatKwd(MshIdx, GmfTetrahedra)))
   {
      if(!(TetTab = malloc( (NmbTet+1) * sizeof(TetSct))))
         exit(1);

      GmfGetBlock(MshIdx, GmfTetrahedra, 1, NmbTet, 0, NULL, NULL,
                  GmfIntVec, 4,  TetTab[1].idx,  TetTab[ NmbTet ].idx,
                  GmfInt,       &TetTab[1].ref, &TetTab[ NmbTet ].ref);

      for(i=1;i<=NmbTet;i++)
         for(j=0;j<4;j++)
            hsh_AddItem(HshIdx, HshTet, TetTab[i].idx[j], i, 1);

      printf("%d tets read and hashed\n", NmbTet);
   }


   // Check existence, allocate, read and hash the pyramids
   if((NmbPyr = (int)GmfStatKwd(MshIdx, GmfPyramids)))
   {
      if(!(PyrTab = malloc( (NmbPyr+1) * sizeof(PyrSct))))
         exit(1);

      GmfGetBlock(MshIdx, GmfPyramids, 1, NmbPyr, 0, NULL, NULL,
                  GmfIntVec, 5,  PyrTab[1].idx,  PyrTab[ NmbPyr ].idx,
                  GmfInt,       &PyrTab[1].ref, &PyrTab[ NmbPyr ].ref);

      for(i=1;i<=NmbPyr;i++)
         for(j=0;j<5;j++)
            hsh_AddItem(HshIdx, HshPyr, PyrTab[i].idx[j], i, 1);

      printf("%d pyramids read and hashed\n", NmbPyr);
   }


   // Check existence, allocate, read and hash the prisms
   if((NmbPri = (int)GmfStatKwd(MshIdx, GmfPrisms)))
   {
      if(!(PriTab = malloc( (NmbPri+1) * sizeof(PriSct))))
         exit(1);

      GmfGetBlock(MshIdx, GmfPrisms, 1, NmbPri, 0, NULL, NULL,
                  GmfIntVec, 6,  PriTab[1].idx,  PriTab[ NmbPri ].idx,
                  GmfInt,       &PriTab[1].ref, &PriTab[ NmbPri ].ref);

      for(i=1;i<=NmbPri;i++)
         for(j=0;j<6;j++)
            hsh_AddItem(HshIdx, HshPri, PriTab[i].idx[j], i, 1);

      printf("%d prisms read and hashed\n", NmbPri);
   }


   // Check existence, allocate, read and hash the hexes
   if((NmbHex = (int)GmfStatKwd(MshIdx, GmfHexahedra)))
   {
      if(!(HexTab = malloc( (NmbHex+1) * sizeof(HexSct))))
         exit(1);

      GmfGetBlock(MshIdx, GmfHexahedra, 1, NmbHex, 0, NULL, NULL,
                  GmfIntVec, 8,  HexTab[1].idx,  HexTab[ NmbHex ].idx,
                  GmfInt,       &HexTab[1].ref, &HexTab[ NmbHex ].ref);

      for(i=1;i<=NmbHex;i++)
         for(j=0;j<8;j++)
            hsh_AddItem(HshIdx, HshHex, HexTab[i].idx[j], i, 1);

      printf("%d hexes read and hashed\n", NmbHex);
   }

   GmfCloseMesh(MshIdx);


   /* Allocate a surface mesh with maximum sizes */

   if(!(TriTab = calloc( 4*NmbTet + 4*NmbPyr + 3*NmbPri + 1, sizeof(TriSct))))
      exit(1);

   if(!(QadTab = calloc( NmbPyr + 3*NmbPri + 6*NmbHex + 1, sizeof(QadSct))))
      exit(1);

   // Create a surface mesh file and write the vertices
   if(!(MshIdx = GmfOpenMesh("../sample_meshes/surface.meshb", GmfWrite, 3, 3)))
      exit(1);

   GmfSetKwd(MshIdx, GmfVertices, NmbVer);
   GmfSetBlock(MshIdx, GmfVertices, 1, NmbVer, 0, NULL, NULL,
               GmfDoubleVec, 3, &VerTab[1].crd, &VerTab[ NmbVer ].crd,
               GmfInt,          &VerTab[1].ref, &VerTab[ NmbVer ].ref);

   // Search for unique triangles and quads which are the boundary ones
   if(NmbTet)
   {
      for(i=1;i<=NmbTet;i++)
         for(j=0;j<4;j++)
         {
            for(k=0;k<3;k++)
               VerFac[k] = TetTab[i].idx[ tettvpf[j][k] ];

            if(hsh_GetItem(HshIdx, 0, HshAny, 3, VerFac, &EleTab, &TypTab) == 1)
            {
               NmbTri++;

               for(k=0;k<3;k++)
                  TriTab[ NmbTri ].idx[k] = VerFac[k];
            }
         }

      printf("Tets parsed     : NmbTri = %d, NmbQad = %d\n", NmbTri, NmbQad);
   }

   if(NmbPyr)
   {
      for(i=1;i<=NmbPyr;i++)
         for(j=0;j<5;j++)
         {
            if(pyrtvpf[j][3] == -1)
            {
               for(k=0;k<3;k++)
                  VerFac[k] = PyrTab[i].idx[ pyrtvpf[j][k] ];

               if(hsh_GetItem(HshIdx, 0, HshAny, 3, VerFac, &EleTab, &TypTab) == 1)
               {
                  NmbTri++;

                  for(k=0;k<3;k++)
                     TriTab[ NmbTri ].idx[k] = VerFac[k];
               }
            }
            else
            {
               for(k=0;k<4;k++)
                  VerFac[k] = PyrTab[i].idx[ pyrtvpf[j][k] ];

               if(hsh_GetItem(HshIdx, 0, HshAny, 4, VerFac, &EleTab, &TypTab) == 1)
               {
                  NmbQad++;

                  for(k=0;k<4;k++)
                     QadTab[ NmbQad ].idx[k] = VerFac[k];
               }
            }
         }

      printf("Pyramids parsed : NmbTri = %d, NmbQad = %d\n", NmbTri, NmbQad);
   }

   if(NmbPri)
   {
      for(i=1;i<=NmbPri;i++)
         for(j=0;j<5;j++)
         {
            if(pritvpf[j][3] == -1)
            {
               for(k=0;k<3;k++)
                  VerFac[k] = PriTab[i].idx[ pritvpf[j][k] ];

               if(hsh_GetItem(HshIdx, 0, HshAny, 3, VerFac, &EleTab, &TypTab) == 1)
               {
                  NmbTri++;

                  for(k=0;k<3;k++)
                     TriTab[ NmbTri ].idx[k] = VerFac[k];
               }
            }
            else
            {
               for(k=0;k<4;k++)
                  VerFac[k] = PriTab[i].idx[ pritvpf[j][k] ];

               if(hsh_GetItem(HshIdx, 0, HshAny, 4, VerFac, &EleTab, &TypTab) == 1)
               {
                  NmbQad++;

                  for(k=0;k<4;k++)
                     QadTab[ NmbQad ].idx[k] = VerFac[k];
               }
            }
         }

      printf("Prisms parsed   : NmbTri = %d, NmbQad = %d\n", NmbTri, NmbQad);
   }

   if(NmbHex)
   {
      for(i=1;i<=NmbHex;i++)
         for(j=0;j<6;j++)
         {
            for(k=0;k<4;k++)
               VerFac[k] = HexTab[i].idx[ hextvpf[j][k] ];

            if(hsh_GetItem(HshIdx, 0, HshAny, 4, VerFac, &EleTab, &TypTab) == 1)
            {
               NmbQad++;

               for(k=0;k<4;k++)
                  QadTab[ NmbQad ].idx[k] = VerFac[k];
            }
         }

      printf("Hexes parsed    : NmbTri = %d, NmbQad = %d\n", NmbTri, NmbQad);
   }

   if(NmbTri)
   {
      GmfSetKwd(MshIdx, GmfTriangles, NmbTri);
      GmfSetBlock(MshIdx, GmfTriangles, 1, NmbTri, 0, NULL, NULL,
                  GmfIntVec, 3, &TriTab[1].idx, &TriTab[ NmbTri ].idx,
                  GmfInt,       &TriTab[1].ref, &TriTab[ NmbTri ].ref);

      free(TriTab);
   }

   if(NmbQad)
   {
      GmfSetKwd(MshIdx, GmfQuadrilaterals, NmbQad);
      GmfSetBlock(MshIdx, GmfQuadrilaterals, 1, NmbQad, 0, NULL, NULL,
                  GmfIntVec, 4, &QadTab[1].idx, &QadTab[ NmbQad ].idx,
                  GmfInt,       &QadTab[1].ref, &QadTab[ NmbQad ].ref);

      free(QadTab);
   }

   printf("hash efficiency = %d bytes / entry\n", hsh_FreeTable(HshIdx));

   free(VerTab);

   if(TetTab)
      free(TetTab);

   if(PyrTab)
      free(PyrTab);

   if(PriTab)
      free(PriTab);

   if(HexTab)
      free(HexTab);

   return(0);
}

//**************************************************************************
//**************************************************************************
//**                                                                      **
//**                                 RsRf                                 **
//**                           by Alexei Soares                           **
//**                                                                      **
//** This program performs many of the functions necessary to             **
//** perform a user defined real space r factor calculation between:      **
//**  1) A map and a reference atomic coordinate file                     **
//**  1) A map and a reference map                                        **
//**                                                                      **
//**************************************************************************
//** In general, the first 5 letters of a keyword are significant         **
//** (all letters in shorter keywords are significant).  In general,      **
//** wherever the words IN or OUT determine the area where an operation   **
//** is to be carried out, the word TOTAL may also be used to indicate    **
//** the operation is to be carried out everywhere, but NO MASK WILL      **
//** BE INPUT WHEN THE KEYWORD 'TOTAL' IS USED:                           **
//**                                                                      **
//**    HELP  => Displays this information to screen.                     **
//**    KEYS  => Displays just the keys and command formats to screen.    **
//**    LIST  => Lists maps and masks in memory, with original load name. **
//**                                                                      **
//**    MAPIN X1 'name'                                                   **
//**          => Input a map of name 'name' into variable location X1.    **
//**             This map will from then on be referenced by its number   **
//**             X1.  All information in variable number X1 will be       **
//**             overwritten with no warning.  Note that the program      **
//**             begins with a call to MAPIN 1, with the name given on    **
//**             the command line.  Consequently, the first action taken  **
//**             by the RsRf program is to input a map with the name given**
//**             at the command line.  ALL OTHER MAPS AND MASKS INPUT TO  **
//**             THE PROGRAM MUST HAVE THE SAME NUMBER OF ROWS, COLUMNS,  **
//**             AND SECTIONS AS THIS FIRST COMMAND LINE INPUT MAP.       **
//**    MASKI Y1 'name'                                                   **
//**          => Input a mask of name 'name' into variable location Y1.   **
//**             This mask will from then on be referenced by its number  **
//**             Y1.                                                      **
//**                                                                      **
//**    SCALE X1 X2 IN/OUT/TOTAL Y1                                       **
//**          => Scale map X1 to map X2 IN or OUT of mask Y1, where X1,   **
//**             X2, and Y1 are numbers of previously loaded maps/masks.  **
//**             Map X1  will be multiplied by a constant such that the   **
//**             sum of density inside/outside of mask Y1 region          **
//**             is the same in both maps.  This is the only operation    **
//**             which modifies the input maps in a typical application.  **
//**          => Example 1:  ?SCALE 1 2 IN 1 (would scale input map       **
//**             number 1 to input map number 2 inside of mask 1).        **
//**          => Example 2:  ?SCALE 1 2 TOTAL (would scale input map      **
//**             number 1 to input map number 2 everywhere).              **
//**    RFAC X1 X2 IN/OUT/TOTAL Y1                                        **
//**          => Find R factor between map X1 and map X2 INR or OUT of    **
//**             mask Y1.                                                 **
//**          => Example:  ?RFAC 1 2 OUT 1                                **
//**             Finds R factor between map 1 and map 2 outside of        **
//**             mask 1.                                                  **
//**    RMS  X1 IN/OUT/TOTAL Y1                                           **
//**          => Find root mean square variance in map density for        **
//**             map X1 IN or OUT of mask Y1.                             **
//**          => Example:  ?RMS 1 OUT 1                                   **
//**             Finds root mean square variance in map density for       **
//**             map 1 outside of mask 1.                                 **
//**    SMEAR X1 X2 X3 N                                                  **
//**          => Smooth map X1 by convolution with linear density         **
//**             sphere and save in location X2.  Memory location X3 is   **
//**             used for temporary calculations.                         **
//**          => Example:  ?SMEAR 1 3 4 3                                 **
//**             Smooths map 1 by spreading out density in one pixel to   **
//**             three additional pixels in all directions, and saves in  **
//**             memory location 3.                                       **
//**                                                                      **
//**    ZERO X1 IN/OUT/TOTAL Y1                                           **
//**          => Assigns zero to all pixels in map X1 which are IN/OUT of **
//**             mask Y1.                                                 **
//**          => Example:  ?ZERO 3 OUT 1 (would assign zero value to all  **
//**             pixels in map 3 inside of mask 1).                       **
//**    CUT X1 IN/OUT/TOTAL Y1 MIN MAX                                    **
//**          => Truncates any value in map X1 IN/OUT of mask Y1 that     **
//**             is above MAX or bellow MIN.                              **
//**          => Example:  ?CUT 3 OUT 1 -0.100 0.600                      **
//**             Truncates all electron densities in map 3 outside of     **
//**             mask 1 with values bellow -0.100 e/A^3 or above          **
//**             0.600 e/A^3.                                             **
//**    MAXOF X1 X2 X3                                                    **
//**          => Combines maps X2 and X3 into map X1 such that each       **
//**             pixel of X1 contains the highest value of X2 and X3      **
//**          => Example:  ?MAXOF 3 1 2                                   **
//**                                                                      **
//**    MAXMS Y1 Y2 Y3                                                    **
//**          => Combines masks Y2 and Y3 into map Y1 such that each      **
//**             pixel of Y1 is 1 if either X2 or X3 is 1.                **
//**          => Example:  ?MAXMS 3 1 2                                   **
//**                                                                      **
//**    MINMS Y1 Y2 Y3                                                    **
//**          => Combines masks Y2 and Y3 into map Y1 such that each      **
//**             pixel of Y1 is 0 if either X2 or X3 is 0.                **
//**          => Example:  ?MINMS 3 1 2                                   **
//**                                                                      **
//**    FLIP  Y1 Y2                                                       **
//**          => Flips 1 and 0 in mask Y2 and saves in mask Y1.           **
//**                                                                      **
//**    ADD X1 X2 IN/OUT/TOTAL Y1                                         **
//**          => Add map X2 to map X1 IN/OUT of mask Y1.                  **
//**          => Example:  ?ADD 3 1 OUT 1                                 **
//**             Adds map 1 to map 3 outside of mask 1 (note that map 1   **
//**             is not modified, but map 3 has all pixels outside of     **
//**             mask 1 incremented by the equivalent values in map 1).   **
//**    SUB X1 X2 IN/OUT/TOTAL Y1                                         **
//**          => Subtract map X2 from map X1 IN/OUT of mask Y1.           **
//**          => Example:  ?SUB 3 1 OUT 1                                 **
//**             Subtracts map 1 from map 3 outside of mask 1.            **
//**    COMB X1 X2 IN/OUT/TOTAL Y1 float                                  **
//**          => Adds X2 times a float to X1 IN/OUT of mask Y1.           **
//**          => Example 1: ? COMB 3 1 OUT 1 -1.0                         **
//**             Subtracts map 1 from map 3 outside of mask 1.            **
//**          => Example 2: ? COMB 3 1 OUT 1 +1.0                         **
//**             Adds map 1 to map 3 outside of mask 1                    **
//**          => Example 3: ? COMB 3 1 OUT 1 +0.25                        **
//**             Adds one quarter of map 1 to map 3 outside of mask 1     **
//**                                                                      **
//**    AVG X1 IN/OUT/TOTAL Y1                                            **
//**          => Finds the average electron density of map X1 IN/OUT of   **
//**             mask Y1 and saves that value in the storage variable.    **
//**          => Example: AVG 1 IN 1                                      **
//**             Stores the average value of map 1 inside mask 1.         **
//**    PLUS X1 IN/OUT/TOTAL Y1 VALUE/float                               **
//**          => Adds a constant to map X1 IN/OUT of mask Y1.             **
//**             The constant is either the value in the storage variable **
//**             (this is done when VALUE is entered) or a float given    **
//**             by the user.                                             **
//**          => Example 1: PLUS 1 IN 1 VALUE                             **
//**             Adds the value of the storage variable to map 1          **
//**             inside mask 1.                                           **
//**          => Example 2: PLUS 1 IN 1 -0.750                            **
//**             Subtracts 0.750 from every pixel in map 1 inside mask 1  **
//**    MULT X1 IN/OUT/TOTAL Y1 float                                     **
//**          => Multiplies map X1 by a float IN/OUT of mask Y1.          **
//**          => Example:  MULT 1 IN 1 0.500                              **
//**             Divides all density in map 1 and within mask 1 by 2.0    **
//**    NEG      Transforms the storage variable into its negative.       **
//**                                                                      **
//**    WRITE X1 'name'                                                   **
//**          => Write map number X1 to file named 'name'.                **
//**          => Example:  ?WRITE 3 out.map                               **
//**    MASKO Y1 'name'                                                   **
//**          => Write mask number Y1 to file named 'name'.               **
//**                                                                      **
//**    GRAY 'name' n X1 X1Start X1Step ... Xn XnStart XnStep             **
//**         x1 x2 y1 y2 z1 z2                                            **
//**          => Write grayscale output.  n maps are output, with density **
//**             and density step size specified for each map.  Output    **
//**             grayscale file will have name 'name'.  Output will       **
//**             consist of a block defined by x1 to x2, y1 to y2, and    **
//**             z1 to z2.   Each of these may be entered as -1 to mean   **
//**             the map min/max.                                         **
//**          => Example:  ?GRAY out.gray 3                               **
//**                               1 -0.50  0.0150                        **
//**                               2 -0.50  0.0150                        **
//**                               3 -0.25  0.0025                        **
//**                              -1 -1                                   **
//**                              -1 -1                                   **
//**                               1  1                                   **
//**             Outputs three maps to a grayscale file, maps 1, 2, and   **
//**             3.  Map 1 begins at density -0.50 e/A^3 and each gray    **
//**             step is equal to 0.0150 density units, and so forth.     **
//**             A single section is output, of the full x and y map.     **
//**                                                                      **
//**    END/QUIT/STOP                                                     **
//**          => Quits program.                                           **
//**                                                                      **
//**************************************************************************
//**************************************************************************


//**************************************************************************
//**                             HEADER FILES                             **
//**************************************************************************

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "stdio.h"
using namespace std;

//**************************************************************************
//**                        DATA TYPE DECLARATIONS                        **
//**************************************************************************

// STRUCTURES

struct   map_header
   {

   long  NC;
   long  NR;
   long  NS;
   long  MODE;
   long  NCSTART;
   long  NRSTART;
   long  NSSTART;
   long  NX;
   long  NY;
   long  NZ;
   float CELL[6];
   long  MAPC;
   long  MAPR;
   long  MAPS;
   float AMIN;
   float AMAX;
   float AMEAN;
   long  ISPG;
   long  NSY;
   float REST[32];
   char  LAB[1000];
   char  SYM[10000];

   };

struct   pdb_file
   {
   int   Num;
   char  Nam[7];
   char  MID[16];
   float x;
   float y;
   float z;
   float O;
   float B;
   char  END[21];
   int   Type;
   float Enum;
   int   X;
   int   Y;
   int   Z;
   };

struct   pdb_info
   {
   char  name[6];
   int   e;
   float r;
   };

//**************************************************************************
//**                           GLOBAL VARIABLES                           **
//**************************************************************************

const float PI       = 3.1415927;
const float DegToRad = ((2.0 * PI)/360);
const float E        = 2.7182818;

int         LAB_LEN  = 800;           // Length of the Header

map_header  MAP_H[21];                // Map Header Information

float       *MAP;                     // The MAPS

char        *MSK;                     // The MASKS

pdb_file    *PDB;                     // The PDB files

pdb_info    PDBdat[100];              // PDB file information
int         DatNum;                   // Number of entries in PDBdat

int         Z_LIM;                    // MAP SIZE:  Z sections
int         Y_LIM;                    //            Y sections
int         X_LIM;                    //            X sections

int         XY_LIM;                   // X * Y (Needed by program)
int         XYZ_LIM;                  // X * Y * Z (Total map size)

int         Z_CELL;                   // Unit cell z-grid points
int         Y_CELL;                   // Unit cell y-grid points
int         X_CELL;                   // Unit cell x-grid points

int         XYZ_CELL;                 // Unit cell x*y*z

float       Z_GRID;                   // Angstrom size of z-grid
float       Y_GRID;                   // Angstrom size of y-grid
float       X_GRID;                   // Angstrom size of x-grid

float       cel_vol;                  // Volume of each unit cell
float       map_vol;                  // Volume of each map
float       vox_vol;                  // Volume of each voxel

float       map_max[21][5];           // Maximum electron density in map
float       map_min[21][5];           // Minimum electron density in map
float       map_avg[21][5];           // Average electron density in map
float       map_tot[21][5];           // Sum of  electron density in map
int         map_num[21][5];           // Number of pixels within map
float       map_var[21][5];           // Variance of density of map
float       map_rms[21][5];           // RMS density (i.e. standard deviation)

int         map_mem;                  // Space for number of maps assigned
int         msk_mem;                  // Space for number of masks assigned
int         pdb_mem;                  // Space for number of PDB files

int         pdb_max;                  // Maximum length of each PDB file
int         pdb_len[10];              // Actual length of each PDB file

int         msk_num_1;                // First mask loaded into memory

float       temp;                     // Used to pass variables to functoins

//**************************************************************************
//**                         FUNCTION PROTOTYPES                          **
//**************************************************************************

// KEYWORDS

void  Help();                         // Prints how to use program

void  Keys();                         // Prints usage keys and formats

int   ReadMap(const char *file, int map1, int mem); // Read map

float ReadMsk(const char *file, int msk1, int mem); // Read mask

int   ReadPDB(const char *file, int pdb1);          // Read pdb file

int   WritePDB(const char *file, int pdb1);         // Write pdb file

int   WriteDAT(const char *file, int pdb1);         // Atom parameters

void  MapHead(int map1);              // Display map  header:

float Scale(int map1, int map2, int zone, int msk1);// Scales maps

float Rfac(int map1, int map2, int zone, int msk1, int type);
                                      // Find map R factor
void  Smear(int map1, int map2, int map3, int N);   // Smooths map

void  Rough(int map1, int map2, int N);             // Calculate map roughness

void  Shape(int msk1, int msk2, int msk3, int map1, float MinDif, int count1, int count2);
         // Modifies mask by expanding until inflection points in all directions

int   Zero(int map1, int zone, int msk1);           // Sets to zero

int   Cut(int map1, int zone, int msk1, float min, float max);
                                      // Cuts below min, above max

void  MaxOf(int map1, int map2, int map3);
                                      // Finds max of 2 maps

void  MaxMs(int msk1, int msk2, int msk3);
                                      // Combines "1" in 2 masks

void  MinMs(int msk1, int msk2, int msk3);
                                      // Combines "0" in 2 masks

void MskCopy(int msk1, int msk2);     // Copy msk1 into msk2
void MapCopy(int map1, int map2);     // Copy map1 into map2

void  Flip(int msk1, int msk2);       // Flips "1" and "0" values

void  MapMod(int map1, int map2, int zone, int msk1, float value);
                                      // Adds/Subtracts maps

float FindParms(int map1, int zone, int msk1);
                                      // Finds max, min, total, and average

float FindRMS  (int map1, int zone, int msk1);
                                      // Finds map rms variance of density

void  MapAdd(int map1, int zone, int msk1, float value);
                                      // Adds value to map

void  MapMult(int map1, int zone, int msk1, float value);
                                      // Multiply map by constant

int   WriteMap(const char *file, int map1);
                                      // Write map to file
float MaskOut(const char *file, int msk1);
                                      // Write mask to file
int   MakeGray(const char *file, int map1); 
                                      // Display eveything to grayscale
void  Integrate(int pdb1, int map1);  
                                      // Integrate pdb file densities

// UTILITY

float cell_volume(float A, float B, float C, float a, float b, float c);
                                      // Calculate cell volume

char  *get_entry();                   // Get entry from keyboard

char  *upper(char input[20]);         // Makes input uppercase

float Ch2float(char input[15]);       // Make character to float

int   zone_find(char input[15]);      // Find zone in mask region

float distance(int X, int Y, int Z);  // x,y,z distance => Angsrtom dist.

float Min(float v1, float v2);        // Returns minimum value

float Abs(float value);               // Absolute value

float Gauss(float S, float X);        // Gaussian value  

int   ReadDATA(const char *file);     // Read pdb data 

int   Same(char *file1, char *file2); // Compare two text files

float Int(int X, int Y, int Z, float r, int map1);
                                      // Integrates density of an atom

// The following three equations all use the convention of transforming
//    real to fractional coordinates such that C-real is aligned to C-frac,
//    and X-real is aligned to X*-frac.

int      C2C_X( int type,                           // Converts real X
                float X1, float Y1, float Z1,       //    variable into
                float A , float B , float C ,       //    fractional X
                float a , float b , float c   );
int      C2C_Y( int type,                           // Converts real Y
                float X1, float Y1, float Z1,       //    variable into
                float A , float B , float C ,       //    fractional Y
                float a , float b , float c   );
int      C2C_Z( int type,                           // Converts real Z
                float X1, float Y1, float Z1,       //    variable into
                float A , float B , float C ,       //    fractional Z
                float a , float b , float c   );

// The following three equations all use the convention of transforming
//    fractional to real coordinates such that C-real is aligned to C-frac,
//    and X-real is aligned to X*-frac.

float    C2C_x( int type,                           // Converts fractional
                float x1, float y1, float z1,       //    X variable into
                float A , float B , float C ,       //    real X
                float a , float b , float c   );
float    C2C_y( int type,                           // Converts fractional
                float x1, float y1, float z1,       //    Y variable into
                float A , float B , float C ,       //    real Y
                float a , float b , float c   );
float    C2C_z( int type,                           // Converts fractional
                float x1, float y1, float z1,       //    Z variable into
                float A , float B , float C ,       //    real Z
                float a , float b , float c   );

//**************************************************************************
//**                            MAIN PROGRAM                              **
//**************************************************************************

main(int argc, char *argv[])
   {

   char  ch;

   int   count1;
   int   count2;
   int   count3;

   int   map1;
   int   map2;
   int   map3;

   int   msk1;
   int   msk2;
   int   msk3;

   int   pdb1;

   int   zone;

   float min;  
   float max;  
   float value;
   float current;

   int   mem = 1;

   char  file[50];
   char  input[20];

   float saved_value;

   char  map[21][50];
   char  msk[11][50];
   char  pdb[21][50];

   cout.precision(4);
   cout.setf(ios::fixed);
   cout.setf(ios::right);

   pdb_mem = 0;

   cout  << "\n\n*********************************************************"
         <<   "\n*       REAL SPACE R FACTR PROGRAM                      *"
         <<   "\n*                by Alexei Soares                       *"
         <<   "\n*            Version 1.0, November 2023                 *"
         <<   "\n*********************************************************";

   if (argc < 2)  {  Help();    return 1;   }      // Not enough load files
                                                   // => print information

   for (count1=0; count1 <=9; count1 ++)
      strcpy(map[count1], "NO NAME");

   for (count1=0; count1 <=4; count1 ++)
      strcpy(msk[count1], "NO NAME");

   for (count1=0; count1 <=4; count1 ++)
      strcpy(pdb[count1], "NO NAME");

   for (count1=0; count1 <=9; count1 ++)
      for (count2=0; count2 <=2; count2 ++)
         {
         map_max[count1][count2] = -1000;
         map_min[count1][count2] = +1000;
         map_avg[count1][count2] = 0;
         map_tot[count1][count2] = 0;
         map_num[count1][count2] = 0;
         map_var[count1][count2] = 0;
         map_rms[count1][count2] = 0;
         }

   map_mem = 3;   if (argc >= 3) map_mem = Ch2float(argv[2]);
   msk_mem = 1;   if (argc >= 4) msk_mem = Ch2float(argv[3]);

   cout  << "\n\n*** Opening PRINCIPAL MAP *****************************\n\n";

   cout  << "   MAPIN =>\n"
         << "   MAPIN => **********************************\n"
         << "   MAPIN => * MAP  STORED IN MEMORY LOCATION * " 
         << " 1\n"
         << "   MAPIN => **********************************\n"
         << "   MAPIN =>\n";

   count1 = (ReadMap(argv[1], 0, 1));              // Read PRINCIPAL MAP

   strcpy (map[0], argv[1]);

   if (count1)
      {
      cout  << "\nERROR:  Cannot open map  " << argv[1] << " !!!\n";
      cout.flush();
      if (count1 == 3) return 1;
      }

   MapHead(0);                                     // PRINCIPAL MAP header

//**************************************************************************
//**                         MAIN PROGRAM LOOP                            **
//**************************************************************************

   do
      {
      cout  << "   MAIN  => Awaiting Keyword? ";
      cin   >> input;                              // Input keyword from user
      strcpy(input, upper(input));                 // Lower case => UPPER CASE

      // *** HELP FUNCTION *************************************************

      if      (!(strncmp(input, "HELP", 4)))       // HELP KEYWORD
         {
         cout  << "   HELP  => Keyword recognized.\n";
         Help();
         cout.flush();
         }
      // *** KEYS FUNCTION *************************************************

      if      (!(strncmp(input, "KEYS", 4)))       // KEYS KEYWORD
         {
         cout  << "   KEYS  => Keyword recognized.\n";
         Keys();
         cout.flush();
         }

      // *** LIST FUNCTION *************************************************

      if      (!(strncmp(input, "LIST", 4)))       // LIST KEYWORD
         {
         cout  << "   LIST  => Keyword recognized.\n";

         cout  << "   LIST  =>\n"
               << "   LIST  => ----------------------------------------\n"
               << "   LIST  => |           MAPS  IN MEMORY            |\n"
               << "   LIST  => |--------------------------------------|\n"
               << "   LIST  => | LOCATION | MAP  NAME                 |\n"
               << "   LIST  => |----------|---------------------------|\n";

         for (count1 = 1; count1 <= map_mem; count1 ++)
            {
            cout  << "   LIST  => |";
            cout.width(6);  cout << count1        << "    | ";
            cout.width(25); cout << map[count1-1] << " |\n";
            }

         cout  << "   LIST  => ----------------------------------------\n"
               << "   LIST  =>\n"
               << "   LIST  => ----------------------------------------\n"
               << "   LIST  => |           MASKS IN MEMORY            |\n"
               << "   LIST  => |--------------------------------------|\n"
               << "   LIST  => | LOCATION | MASK NAME                 |\n"
               << "   LIST  => |----------|---------------------------|\n";

         for (count1 = 1; count1 <= msk_mem; count1 ++)
            {
            cout  << "   LIST  => |";
            cout.width(6);  cout << count1        << "    | ";
            cout.width(25); cout << msk[count1-1] << " |\n";
            }

         cout  << "   LIST  => ----------------------------------------\n"
               << "   LIST  =>\n";

         if (pdb_mem)
            {
            cout  << "   LIST  => ----------------------------------------\n"
                  << "   LIST  => |           PDB FILES IN MEMORY        |\n"
                  << "   LIST  => |--------------------------------------|\n"
                  << "   LIST  => | LOCATION | PDB FILE NAME             |\n"
                  << "   LIST  => |----------|---------------------------|\n";

            for (count1 = 1; count1 <= pdb_mem; count1 ++)
               {
               cout  << "   LIST  => |";
               cout.width(6);  cout << count1        << "    | ";
               cout.width(25); cout << pdb[count1-1] << " |\n";
               }

            cout  << "   LIST  => ----------------------------------------\n"
                  << "   LIST  =>\n";
            }

         cout.flush();

         }

      // *** MAPIN FUNCTION ************************************************

      else if (!(strncmp(input, "MAPIN", 5)))      // MAPIN KEYWORD
         {
         cout  << "   MAPIN => Keyword recognized.\n";
         cout  << "   MAPIN => Map  memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   MAPIN => Name of map  to read? ";
         cin   >> file;

         count1 = (ReadMap(file, map1, 0));        // Read map

         if (count1) cout  << "   MAPIN => CANNOT OPEN FILE!\n";
         if (count1) continue;

         cout  << "   MAPIN =>\n"
               << "   MAPIN => **********************************\n"
               << "   MAPIN => * MAP  STORED IN MEMORY LOCATION * "
               << (map1+1) << "\n"
               << "   MAPIN => **********************************\n"
               << "   MAPIN =>\n";

         strcpy (map[map1], file);

         MapHead(map1);                            // Map header

         cout.flush();
         }

      // *** MASKI FUNCTION ************************************************

      else if (!(strncmp(input, "MASKI", 5)))      // MASKI KEYWORD
         {
         cout  << "   MASKI => Keyword recognized.\n";
         cout  << "   MASKI => Mask memory location (1 to "
               << msk_mem << ")? ";
         cin   >> msk1;   msk1 --;
         cout  << "   MASKI => Name of mask to read? ";
         cin   >> file;

         value  = (ReadMsk(file, msk1, mem));      // Read mask

         if (value < -.1) cout  << "   MASKI => CANNOT OPEN FILE!\n";
         if (value < -.1) continue;

         cout  << "   MASKI => Percent of pixels in mask is " 
               << value << "\n";

         cout  << "   MASKI =>\n"
               << "   MASKI => **********************************\n"
               << "   MASKI => * MASK STORED IN MEMORY LOCATION * "
               << (msk1+1) << "\n"
               << "   MASKI => **********************************\n"
               << "   MASKI =>\n";

         strcpy (msk[msk1], file);

         mem = 0;

         MapHead(map_mem + msk1);                  // Mask header

         cout.flush();
         }

      // *** PDBIN FUNCTION ************************************************

      else if (!(strncmp(input, "PDBIN", 5)))      // PDBIN KEYWORD
         {
         cout  << "   PDBIN => Keyword recognized.\n";
 
         count1 = 0;

         if (!pdb_mem)
            {
            cout  << "   PDBIN => Maximum number of PDB files used? ";
            cin   >> pdb_mem;
            cout  << "   PDBIN => Maximum length of each PDB file? ";
            cin   >> pdb_max;

            PDB = new pdb_file [(pdb_max * pdb_mem) + pdb_mem];

            cout  << "   PDBIN => PDB atom parameter file name? ";
            cin   >> file;

            count1 = (ReadDATA(file));             // Read PDB info
            }

         if (count1)
            {
            cout  << "   PDBIN => CANNOT OPEN FILE!\n";
            pdb_mem = 0;
            }

         if (count1) continue;

         cout  << "   PDBIN => PDB file memory location (1 to "
               << pdb_mem << ")? ";
         cin   >> pdb1;   pdb1 --;
         cout  << "   PDBIN => Name of pdb file to read? ";
         cin   >> file;

         count1 = (ReadPDB(file, pdb1));           // Read PDB file

         if (count1) cout  << "   PDBIN => CANNOT OPEN FILE!\n";
         if (count1) continue;

         cout  << "   PDBIN =>\n"
               << "   PDBIN => **********************************\n"
               << "   PDBIN => * FILE STORED IN MEMORY LOCATION * "
               << (pdb1+1) << "\n"
               << "   PDBIN => **********************************\n"
               << "   PDBIN =>\n";

         strcpy (pdb[pdb1], file);

         cout.flush();

         }

      // *** PDBOU FUNCTION ************************************************

      else if (!(strncmp(input, "PDBOU", 5)))      // PDBOU KEYWORD
         {
         cout  << "   PDBOU => Keyword recognized.\n";
 
         cout  << "   PDBOU => PDB file memory location (1 to "
               << pdb_mem << ")? ";
         cin   >> pdb1;   pdb1 --;
         cout  << "   PDBOU => Name of save file?        ";
         cin   >> file;

         count1 = (WritePDB(file, pdb1));          // Write PDB file

         if (count1) cout  << "   PDBOU => CANNOT OPEN FILE!\n";
         if (count1) continue;

         cout  << "   PDBIN => File number " << (pdb1+1) << " stored as "
               << file << "\n";

         strcpy (pdb[pdb1], file);

         cout.flush();

         }

      // *** PDBDA FUNCTION ************************************************

      else if (!(strncmp(input, "PDBDA", 5)))      // PDBDA KEYWORD
         {
         cout  << "   PDBDA => Keyword recognized.\n";

         cout  << "   PDBDA => PDB file memory location (1 to "
               << pdb_mem << ")? ";
         cin   >> pdb1;   pdb1 --;
         cout  << "   PDBDA => Name of save file?        ";
         cin   >> file;

         count1 = (WriteDAT(file, pdb1));          // Write PDB file

         if (count1) cout  << "   PDBDA => CANNOT OPEN FILE!\n";
         if (count1) continue;

         cout  << "   PDBDA => File number " << (pdb1+1) << " stored as "
               << file << "\n";

         cout.flush();

         }

      // *** SCALE FUNCTION ************************************************

      else if (!(strncmp(input, "SCALE", 5)))      // SCALE KEYWORD
         {
         cout  << "   SCALE => Keyword recognized.\n";
         cout  << "   SCALE => Map to be scaled memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   SCALE => Reference map memory location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;

         zone = zone_find("SCALE => ");

         if (zone != 2)
            {
            cout  << "   SCALE => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         value = Scale(map1, map2, zone, msk1);

         cout  << "   SCALE => Scale factor is " << value << "\n";
         cout  << "   SCALE => Scale operation completed.\n";

         cout.flush();
         }

      // *** RFAC FUNCTION *************************************************

      else if (!(strncmp(input, "RFAC", 4)))       // RFAC KEYWORD
         {
         cout  << "   RFAC  => Keyword recognized.\n";
         cout  << "   RFAC  => Map to be compared location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   RFAC  => Reference map memory location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;

         zone = zone_find("RFAC  => ");

         if (zone != 2)
            {
            cout  << "   RFAC  => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         cout  << "   RFAC  => Which r-factor do you want?\n"
               << "   RFAC  =>   1) Avg diff / Map 1 average\n"
               << "   RFAC  =>   2) Avg diff / Map 2 average\n"
               << "   RFAC  =>   3) Avg diff / ((Avg1 + Avg2)/2)\n"
               << "   RFAC  =>   4) Avg diff / Map 1 RMS\n"
               << "   RFAC  =>   5) Avg diff / Map 2 RMS\n"
               << "   RFAC  =>   6) Avg diff / ((RMS1 + RMS2)/2)\n";

         cin   >> count1;

         value = Rfac(map1, map2, zone, msk1, count1);

         cout  << "   RFAC  => ********************************************\n"
               << "   RFAC  => * R FACTOR IS (ZONE " << zone 
               << ", TYPE " << count1 << ")";
         cout.width(12); cout << value << " *\n";
         cout  << "   RFAC  => ********************************************\n";

         cout.flush();
         }

      // *** RMS FUNCTION **************************************************

      else if (!(strncmp(input, "RMS", 3)))        // RMS KEYWORD
         {
         cout  << "   RMS   => Keyword recognized.\n";
         cout  << "   RMS   => Map location (1 to " << map_mem << ")? ";
         cin   >> map1;   map1 --;

         zone = zone_find("RMS   => ");

         if (zone != 2)
            {
            cout  << "   RMS   => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         value = FindRMS(map1, zone, msk1);

         cout  << "   RMS   => ***********************************\n"
               << "   RMS   => * RMS VALUE IS (ZONE " << zone << ")";
         cout.width(12); cout << value << " *\n";
         cout  << "   RMS   => ***********************************\n";

         cout.flush();
         }

      // *** SMEAR FUNCTION ************************************************

      else if (!(strncmp(input, "SMEAR", 5)))      // SMEAR KEYWORD
         {
         cout  << "   SMEAR => Keyword recognized.\n";
         cout  << "   SMEAR => Map to be smoothed memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   SMEAR => Save new map in memory location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;
         cout  << "   SMEAR => Temporary calculation memory (1 to "
               << map_mem << ")? ";
         cin   >> map3;   map3 --;
         cout  << "   SMEAR => Smear to how many pixels to each side? ";
         cin   >> count1;

         Smear(map1, map2, map3, count1);

         cout  << "   SMEAR => Smoothing completed.\n";

         if (!(strcmp(map[map2], "NO NAME")))
            strcpy(map[map2], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** OCCUP FUNCTION ************************************************

      else if (!(strncmp(input, "OCCUP", 5)))      // OCCUP KEYWORD
         {
         cout  << "   OCCUP => Keyword recognized.\n";
         cout  << "   OCCUP => PDB file number for occupancy (1 to "
               << pdb_mem << ")? ";
         cin   >> pdb1;   pdb1 --;
         cout  << "   OCCUP => Map number to integrate over (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         Integrate(pdb1, map1);

         cout  << "   OCCUP => Occupancy calculated.\n";

         cout.flush();
         }

      // *** ZERO FUNCTION *************************************************

      else if (!(strncmp(input, "ZERO", 4)))       // ZERO KEYWORD
         {
         cout  << "   ZERO  => Keyword recognized.\n";
         cout  << "   ZERO  => Set which map to zero (map location 1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         zone = zone_find("ZERO  => ");

         if (zone != 2)
            {
            cout  << "   ZERO  => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         count1 = Zero(map1, zone, msk1);

         cout  << "   ZERO =>  Map set to zero.\n";
         cout  << "   ZERO =>  Number of pixels changed: " << count1 << "\n";
         cout  << "   ZERO =>  Percent of map changed:   " 
               << ((count1 * 1.0)/(XYZ_LIM * 1.0)) << "\n";

         cout.flush();
         }

      // *** CUT FUNCTION **************************************************

      else if (!(strncmp(input, "CUT", 3)))        // CUT KEYWORD
         {
         cout  << "   CUT   => Keyword recognized.\n";
         cout  << "   CUT   => Density cutoff on map (map location 1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         zone = zone_find("CUT   => ");

         if (zone != 2)
            {
            cout  << "   CUT   => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         cout  << "   CUT   => Minimum density cutoff? ";
         cin   >> min;
         cout  << "   CUT   => Maximum density cutoff? ";
         cin   >> max;

         count1 = Cut(map1, zone, msk1, min, max);

         cout  << "   CUT   => Density cutoff applied.\n";
         cout  << "   CUT   => Number of pixels changed: " << count1 << "\n"; 
         cout  << "   CUT   => Percent of map changed:   " 
               << ((count1 * 1.0)/(XYZ_LIM * 1.0)) << "\n";

         cout.flush();
         }


      // *** MAXOF FUNCTION ************************************************

      else if (!(strncmp(input, "MAXOF", 5)))      // MAXOF KEYWORD
         {
         cout  << "   MAXOF => Keyword recognized.\n";
         cout  << "   MAXOF => Which map will be generated (map location 1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         cout  << "   MAXOF => The maximum of which two maps go to map " 
               << (map1 + 1) << "? ";
         cin   >> map2;   map2 --;
         cin   >> map3;   map3 --;

         MaxOf(map1, map2, map3);

         cout  << "   MAXOF => New map generated.\n";

         if (!(strcmp(map[map1], "NO NAME")))
            strcpy(map[map1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** MAXMS FUNCTION ************************************************

      else if (!(strncmp(input, "MAXMS", 5)))      // MAXMS KEYWORD
         {
         cout  << "   MAXMS => Keyword recognized.\n";
         cout  << "   MAXMS => Which mask will be generated (location 1 to "
               << msk_mem << ")? ";
         cin   >> msk1;   msk1 --;

         cout  << "   MAXMS => The maximum of which two masks go to mask "
               << (msk1 + 1) << "? ";
         cin   >> msk2;   msk2 --;
         cin   >> msk3;   msk3 --;

         MaxMs(msk1, msk2, msk3);

         cout  << "   MAXMS => New mask generated.\n";

         if (!(strcmp(msk[msk1], "NO NAME")))
            strcpy(msk[msk1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** MINMS FUNCTION ************************************************

      else if (!(strncmp(input, "MINMS", 5)))      // MINMS KEYWORD
         {
         cout  << "   MINMS => Keyword recognized.\n";
         cout  << "   MINMS => Which mask will be generated (location 1 to "
               << msk_mem << ")? ";
         cin   >> msk1;   msk1 --;

         cout  << "   MINMS => The minimum of which two masks go to mask "
               << (msk1 + 1) << "? ";
         cin   >> msk2;   msk2 --;
         cin   >> msk3;   msk3 --;

         MinMs(msk1, msk2, msk3);

         cout  << "   MINMS => New mask generated.\n";

         if (!(strcmp(msk[msk1], "NO NAME")))
            strcpy(msk[msk1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** FLIP FUNCTION *************************************************

      else if (!(strncmp(input, "FLIP", 4)))       // FLIP KEYWORD
         {
         cout  << "   FLIP  => Keyword recognized.\n";

         cout  << "   MINMS => Save new mask in which location? ";
         cin   >> msk1;   msk1 --;

         cout  << "   FLIP  => Which mask will have 0 and 1 swapped? ";
         cin   >> msk2;   msk2 --;

         Flip(msk1, msk2);

         cout  << "   FLIP  => New mask generated.\n";

         if (!(strcmp(msk[msk1], "NO NAME")))
            strcpy(msk[msk1], "COMPUTER GENERATED");

         cout.flush();
         }


      // *** MASK COPY FUNCTION ********************************************

      else if (!(strncmp(input, "MSKC", 4)))       // MSK COPY KEYWORD
         {
         cout  << "   MSKC  => Keyword recognized.\n";

         cout  << "   MSKC  => Which mask to copy? ";
         cin   >> msk1;   msk1 --;

         cout  << "   MSKC  => Copy mask into which mask location?   ";
         cin   >> msk2;   msk2 --;

         MskCopy(msk1, msk2);     // Copy msk1 into msk2

         cout  << "   MSKC  => New mask generated.\n";
	 }

      // *** ADD FUNCTION **************************************************

      else if (!(strncmp(input, "ADD", 3)))        // ADD KEYWORD
         {
         cout  << "   ADD   => Keyword recognized.\n";
         cout  << "   ADD   => Map to be modified memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   ADD   => Map to be added memory location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;

         zone = zone_find("ADD   => ");

         if (zone != 2)
            {
            cout  << "   ADD   => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         MapMod(map1, map2, zone, msk1, +1);

         cout  << "   ADD   => Map addition completed.\n";

         if (!(strcmp(map[map1], "NO NAME")))
            strcpy(map[map1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** SUB FUNCTION **************************************************

      else if (!(strncmp(input, "SUB", 3)))        // SUB KEYWORD
         {
         cout  << "   SUB   => Keyword recognized.\n";
         cout  << "   SUB   => Map to be modified memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   SUB   => Map to be subtracted location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;

         zone = zone_find("SUB   => ");

         if (zone != 2)
            {
            cout  << "   SUB   => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         MapMod(map1, map2, zone, msk1, -1);

         cout  << "   SUB   => Map subtraction completed.\n";

         if (!(strcmp(map[map1], "NO NAME")))
            strcpy(map[map1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** COMB FUNCTION *************************************************

      else if (!(strncmp(input, "COMB", 4)))       // COMB KEYWORD
         {
         cout  << "   COMB  => Keyword recognized.\n";
         cout  << "   COMB  => Map to be modified memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   COMB  => Map to be combined location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;

         zone = zone_find("COMB  => ");

         if (zone != 2)
            {
            cout  << "   COMB  => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         cout  << "   COMB  => Multiplicative factor for combining maps? ";
         cin   >> value;

         MapMod(map1, map2, zone, msk1, value);

         cout  << "   SUB   => Maps combined successfully.\n";

         if (!(strcmp(map[map1], "NO NAME")))
            strcpy(map[map1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** AVG FUNCTION **************************************************

      else if (!(strncmp(input, "AVG", 3)))        // AVG KEYWORD
         {
         cout  << "   AVG   => Keyword recognized.\n";
         cout  << "   AVG   => Map to be used memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         zone = zone_find("AVG   => ");

         if (zone != 2)
            {
            cout  << "   AVG   => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         saved_value = FindParms(map1, zone, msk1);

         cout  << "   AVG   =>\n"
               << "   AVG   => ****************************************\n"
               << "   AVG   => *********** MAP PARAMETERS *************\n"
               << "   AVG   => ****************************************\n";

         cout  << "   AVG   => **  MAP MAXIMUM:  ";
         cout.width(18); cout << map_max[map1][zone]; cout << "  **\n";

         cout  << "   AVG   => **  MAP MINIMUM:  ";
         cout.width(18); cout << map_min[map1][zone]; cout << "  **\n";

         cout  << "   AVG   => **  MAP AVERAGE:  ";
         cout.width(18); cout << map_avg[map1][zone]; cout << "  **\n";

         cout  << "   AVG   => **  MAP TOTAL:    ";
         cout.width(18); cout << map_tot[map1][zone]; cout << "  **\n";

         cout  << "   AVG   => **  PIXEL COUNT:  ";
         cout.width(18); cout << map_num[map1][zone]; cout << "  **\n";

         cout  << "   AVG   => ****************************************\n"
               << "   AVG   => ****************************************\n"
               << "   AVG   =>\n";

         cout  << "   AVG   => Map average saved in memory variable.\n";

         cout.flush();
         }


      // *** MDIF FUNCTION **************************************************

      else if (!(strncmp(input, "MDIF", 4)))       // Minimize difference keyword; this function carried out inline (no call to MDIF)
         {
         cout  << "   MDIF  => Keyword recognized.\n";
         cout  << "   MDIF  => Map to be compared location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   MDIF  => Reference map memory location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;
         cout  << "   MDIF  => Temporary calculation map memory location (1 to "
               << map_mem << ")? ";
         cin   >> map3;   map3 --;

         zone = zone_find("MDIF  => ");

         if (zone != 2)
            {
            cout  << "   MDIF  => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         current = 100;

         cout  << "   MDIF  => Making map copy\n";
         MapCopy(map1,map3);
         MapMult(map3, zone, msk1, 0.50);
         
	 for (count2 = 0; count2 <=10; count2 ++)
            {
            value = Rfac(map3, map2, zone, msk1, 7);
	    if (value < current)
               {
               current = value;
               count3 = count2;
	       }
            MapMult(map3, zone, msk1, 1.10);
	    }

         MapCopy(map1,map3);
         temp = (0.45*pow(1.10, count3));
         MapMult(map3, zone, msk1, temp);

         current = 100;

         for (count2 = 0; count2 <=10; count2 ++)
            {
            value = Rfac(map3, map2, zone, msk1, 7);
            if (value < current)
               {
               current = value;
               count3 = count2;
               }
            MapMult(map3, zone, msk1, 1.02);
            }

         
         cout  << "   MDIF  => ********************************************\n"
               << "   MDIF  => * DIFFERENCE MINIMIZED WHEN MAP " << map1 << " IS MULTIPLIED BY ";
         cout.width(12); cout << (temp*pow(1.02,count3)) << " *\n";
         cout  << "   MDIF  => ********************************************\n";

         cout.flush();

         saved_value = (temp*pow(1.02,count3));

         cout  << "   MDIF  => Optimal scale saved in memory variable.\n";

         cout.flush();
         }


      // *** PLUS FUNCTION *************************************************

      else if (!(strncmp(input, "PLUS", 4)))       // PLUS KEYWORD
         {
         cout  << "   PLUS  => Keyword recognized.\n";
         cout  << "   PLUS  => Add constant to map memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         zone = zone_find("PLUS  => ");

         if (zone != 2)
            {
            cout  << "   PLUS  => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         cout  << "   PLUS  => Add memory variable (VALUE) or enter "
               << "value to add? ";
         cin   >> file;

         if (file[0] == 'V' || file[0] == 'v') value = saved_value;
         else value = Ch2float(file);

         MapAdd(map1, zone, msk1, value);
         
         cout  << "   PLUS  => Constant " << value << " added to map.\n";

         if (!(strcmp(map[map1], "NO NAME")))
            strcpy(map[map1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** MULT **********************************************************

      else if (!(strncmp(input, "MULT", 4)))       // MULT KEYWORD
         {
         cout  << "   MULT  => Keyword recognized.\n";
         cout  << "   MULT  => Multiply which map memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         zone = zone_find("MULT  => ");

         if (zone != 2)
            {
            cout  << "   MULT  => Mask memory location (1 to "
                  << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;
            }
         else msk1 = 0;

         cout  << "   MULT  => Add memory variable (VALUE) or enter "
               << "value to add? ";
         cin   >> file;

         if (file[0] == 'V' || file[0] == 'v') value = saved_value;
         else value = Ch2float(file);

         MapMult(map1, zone, msk1, value);

         cout  << "   MULT  => Map multiplied by constant " << value << "\n";

         if (!(strcmp(map[map1], "NO NAME")))
            strcpy(map[map1], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** ROUGH *********************************************************

      else if (!(strncmp(input, "ROUG", 4)))       // ROUGH KEYWORD
         {
         cout  << "   ROUGH => Keyword recognized.\n";
         cout  << "   ROUGH => Map to have roughness calculated memory location (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   ROUGH => Save new map in memory location (1 to "
               << map_mem << ")? ";
         cin   >> map2;   map2 --;
         cout  << "   ROUGH => Roughness callculation pixel radius (integer max 10)? ";
         cin   >> count1;

         Rough(map1, map2, count1);

         cout  << "   ROUGH => Roughness calculated.\n";

         if (!(strcmp(map[map2], "NO NAME")))
            strcpy(map[map2], "COMPUTER GENERATED");

         cout.flush();
         }

      // *** SHAPE *********************************************************

      else if (!(strncmp(input, "SHAP", 4)))       // SHAPE KEYWORD
         {
         cout  << "   SHAPE => Keyword recognized.\n";
         cout  << "   SHAPE => Mask to modify memory location (1 to "
               << msk_mem << ")? ";
         cin   >> msk1;   msk1 --;
         cout  << "   SHAPE => Save new mask in memory location (1 to "
               << msk_mem << ")? ";
         cin   >> msk2;   msk2 --;
         cout  << "   SHAPE => Temporary calculation memory (1 to "
               << msk_mem << ")? ";
         cin   >> msk3;   msk3 --;
         cout  << "   SHAPE => Map location for countouring values (1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;

         cout  << "   SHAPE => Minimum difference to qualify for inclusion? ";
         cin   >> temp;

         cout  << "   SHAPE => Max cycles of shape fitting (integer max 100)? ";
         cin   >> count1; if (count1 > 100) count1 = 100;
         cout  << "   SHAPE => Min constriction size in pixels (integer)?";
         cin   >> count2;

         Shape(msk1, msk2, msk3, map1, temp, count1, count2);

         cout  << "   SHAPE => Mask contouring complete.\n";

         if (!(strcmp(msk[msk2], "NO NAME")))
            strcpy(msk[msk2], "COMPUTER GENERATED");

         cout.flush();
         }


      // *** NEG ***********************************************************

      else if (!(strncmp(input, "NEG", 3)))        // NEG KEYWORD
         {
         cout  << "   NEG   => Keyword recognized.\n";
         cout  << "   NEG   => Memory variable multiplied by -1.\n"
               << "   NEG   => Old value in memory: " << saved_value << "\n"
               << "   NEG   => New value in memory: " << (saved_value * -1)
               << "\n";
         saved_value = (saved_value * -1);

         cout.flush();
         }

      // *** NAME **********************************************************

      else if (!(strncmp(input, "NAME", 4)))       // NAME KEYWORD
         {
         cout  << "   NAME  => Keyword recognized.\n";
         cout  << "   NAME  => Change name of Map, mAsk, or Pdb file (M,A,P)? ";
         cin   >> ch;

         if      ((ch == 'M') || (ch == 'm'))
            {
            cout  << "   NAME  => Which file to use (memory location 1 to "
               << map_mem << ")? ";
            cin   >> map1;   map1 --;

            cout  << "   NAME  => Old name: " <<  map[map1] << "\n";
            cout  << "   NAME  => New name: ";

            cout.flush();

            fgets(map[map1],30,stdin);
            }

         else if ((ch == 'A') || (ch == 'a'))
            {
            cout  << "   NAME  => Which file to use (memory location 1 to "
               << msk_mem << ")? ";
            cin   >> msk1;   msk1 --;

            cout  << "   NAME  => Old name: " <<  msk[msk1] << "\n";
            cout  << "   NAME  => New name: ";

            cout.flush();

            fgets(msk[msk1],30,stdin);
            }

         else if ((ch == 'P') || (ch == 'p'))
            {
            cout  << "   NAME  => Which file to use (memory location 1 to "
               << pdb_mem << ")? ";
            cin   >> pdb1;   pdb1 --;

            cout  << "   NAME  => Old name: " <<  pdb[pdb1] << "\n";
            cout  << "   NAME  => New name: ";

            cout.flush();

            fgets(pdb[pdb1],30,stdin);
            }

         else
            {
            cout  << "   NAME  => Unknown file type, must be M, A or P.";
            continue;
            }

         cout.flush();
         }

      // *** WRITE FUNCTION ************************************************

      else if (!(strncmp(input, "WRITE", 5)))      // WRITE KEYWORD
         {
         cout  << "   WRITE => Keyword recognized.\n";
         cout  << "   WRITE => Which map to write (memory location 1 to "
               << map_mem << ")? ";
         cin   >> map1;   map1 --;
         cout  << "   WRITE => Filename for map? ";
         cin   >> file;

         count1 = (WriteMap(file, map1));          // Write map 

         if (count1) cout  << "   WRITE => Failed to write map.\n";
         if (count1) continue;

         cout  << "   WRITE => Map " << (map1+1) << " written to file: "
               << file << "\n";

         strcpy(map[map1], file);

         cout.flush();
         }

      // *** MASKO FUNCTION ************************************************

      else if (!(strncmp(input, "MASKO", 5)))      // MASKO KEYWORD
         {
         cout  << "   MASKO => Keyword recognized.\n";
         cout  << "   MASKO => Which mask to write (memory location 1 to "
               << msk_mem << ")? ";
         cin   >> msk1;   msk1 --;
         cout  << "   MASKO => Filename for mask? ";
         cin   >> file;

         value = MaskOut(file, msk1);

         if (value < -.1) cout  << "   MASKO => Failed to write mask.";
         if (value < -.1) continue;

         cout  << "   MASKO => Percent of pixels in mask is "
               << value << "\n";

         cout  << "   MASKO => Mask " << (msk1+1) << " written to file: "
               << file << "\n";

         strcpy(msk[msk1], file);

         cout.flush();
         }

      // *** GRAY FUNCTION *************************************************

      else if (!(strncmp(input, "GRAY", 4)))       // GRAY KEYWORD
         {
         cout  << "   GRAY  => Keyword recognized.\n";
         cout  << "   GRAY  => Filename to write to? ";
         cin   >> file;
         cout  << "   GRAY  => How many maps to write? ";
         cin   >> map1;   map1 --;

         MakeGray(file, map1);

         cout  << "   GRAY  => Grayscale file written.\n";

         cout.flush();
         }

      }

   while ( (strncmp(input, "END" , 3)) && 
           (strncmp(input, "QUIT", 4)) &&
           (strncmp(input, "STOP", 4)) &&    
           (strncmp(input, "EXIT", 4))    );

   cout  << "\n\nALL DONE !!!\n\n";

   return 0;

   }

//**************************************************************************
//** READ MAP function:  Read in a map file and stores it in *MAP         **
//**************************************************************************

int   ReadMap(const char *file, int map1, int mem)
   {

   int   countz;
   int   county;
   int   countx;

   float frac_vol;

   cout.setf(ios::fixed);
   cout.setf(ios::right);

   FILE  *read1;

   if ((read1 = fopen(file, "rb")) == NULL)        // Read failure
      return 1;

   // ********************   READ MAP HEADER ****************************

   fread( &MAP_H[map1].NC     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].NR     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].NS     , sizeof(int) ,    1, read1);

   fread( &MAP_H[map1].MODE   , sizeof(int) ,    1, read1);

   fread( &MAP_H[map1].NCSTART, sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].NRSTART, sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].NSSTART, sizeof(int) ,    1, read1);

   fread( &MAP_H[map1].NX     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].NY     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].NZ     , sizeof(int) ,    1, read1);

   fread(  MAP_H[map1].CELL   , sizeof(float),   6, read1);
   
   fread( &MAP_H[map1].MAPC   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].MAPR   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].MAPS   , sizeof(int) ,    1, read1);
   
   fread( &MAP_H[map1].AMIN   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].AMAX   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map1].AMEAN  , sizeof(int) ,    1, read1);

   fread( &MAP_H[map1].ISPG   , sizeof(int) ,    1, read1);

   fread( &MAP_H[map1].NSY    , sizeof(int) ,    1, read1);

   fread(  MAP_H[map1].REST   , sizeof(float),   32, read1);

   fread(  MAP_H[map1].LAB    , sizeof(char) , LAB_LEN, read1);
   fread(  MAP_H[map1].SYM    , sizeof(char) , MAP_H[map1].NSY, read1);

   // *******IF FIRST CALL TO FUNCTION, ASSIGN MEMORY AND MAP SIZE *********

   if(mem)
      {

      cout << "   MAPIN => Attempting to assign memory for all maps ...\n";

      X_LIM    = MAP_H[map1].NC;                   // MAP SIZE:  X Sections
      Y_LIM    = MAP_H[map1].NR;                   //            Y Sections
      Z_LIM    = MAP_H[map1].NS;                   //            Z Sections

      XY_LIM   = (X_LIM * Y_LIM);
      XYZ_LIM  = (X_LIM * Y_LIM * Z_LIM);

      X_CELL   = MAP_H[map1].NX;                   // CELL SIZE: X Sections
      Y_CELL   = MAP_H[map1].NY;                   //            Y Sections
      Z_CELL   = MAP_H[map1].NZ;                   //            Z Sections

      XYZ_CELL = (X_CELL * Y_CELL * Z_CELL);

      X_GRID   = MAP_H[map1].CELL[0]/X_CELL;       // GRID size, Angstroms
      Y_GRID   = MAP_H[map1].CELL[1]/Y_CELL;
      Z_GRID   = MAP_H[map1].CELL[2]/Z_CELL;

      MAP = new float[(XYZ_LIM * map_mem) + map_mem];

      if(!MAP)
         {                                         // Not enough memory
         cout  << "\nINSUFFICIENT MEMORY!!!\n";
         return 3;
         }

      cout << "   MAPIN => Memory assigned ...\n";

      cout << "   MAPIN => Setting all pixels to zero.\n";

      for (countx = 0; countx < (XYZ_LIM * map_mem + map_mem); countx ++)
         MAP[countx] = 0;


      // *********** CALCULATE UNIT CELL VOLUME, SHOULD ALL BE EQUAL *******

      frac_vol = (XYZ_LIM * 1.0)/(XYZ_CELL * 1.0);

      cel_vol =  cell_volume(   MAP_H[map1].CELL[0],
                                MAP_H[map1].CELL[1],
                                MAP_H[map1].CELL[2],
                                MAP_H[map1].CELL[3],
                                MAP_H[map1].CELL[4],
                                MAP_H[map1].CELL[5]   );

      map_vol = cel_vol * frac_vol;

      vox_vol = cel_vol/XYZ_CELL;

      cout.precision(4);

      cout  << "   MAPIN => Grid X size      = " << X_GRID   << "\n"
            << "   MAPIN => Grid Y size      = " << Y_GRID   << "\n"
            << "   MAPIN => Grid Z size      = " << Z_GRID   << "\n"
            << "   MAPIN => Grid volume      = " << vox_vol  << "\n";

      cout  << "   MAPIN => ----------------------------------------------\n"
            << "   MAPIN => | PARAMETER          | UNIT CELL |    MAP    |\n"
            << "   MAPIN => |--------------------|-----------|-----------|\n";

      cout  << "   MAPIN => | X in Grid Units    |";
      cout.width(11);  cout << X_CELL                        << "|";
      cout.width(11);  cout << X_LIM                         << "|\n";

      cout  << "   MAPIN => | Y in Grid Units    |";
      cout.width(11);  cout << Y_CELL                        << "|";
      cout.width(11);  cout << Y_LIM                         << "|\n";

      cout  << "   MAPIN => | Z in Grid Units    |";
      cout.width(11);  cout << Z_CELL                        << "|";
      cout.width(11);  cout << Z_LIM                         << "|\n";

      cout  << "   MAPIN => | X in Angstroms     |";
      cout.width(11);  cout << X_CELL * X_GRID               << "|";
      cout.width(11);  cout << X_LIM  * X_GRID               <<"|\n";

      cout  << "   MAPIN => | Y in Angstroms     |";
      cout.width(11);  cout << Y_CELL * Y_GRID               << "|";
      cout.width(11);  cout << Y_LIM  * Y_GRID               <<"|\n";

      cout  << "   MAPIN => | Z in Angstroms     |";
      cout.width(11);  cout << Z_CELL * Z_GRID               << "|";
      cout.width(11);  cout << Z_LIM  * Z_GRID               <<"|\n";

      cout.precision(0);

      cout  << "   MAPIN => | Voxel number       |";
      cout.width(11);  cout << XYZ_CELL                      << "|";
      cout.width(11);  cout << XYZ_LIM                       << "|\n";

      cout  << "   MAPIN => | Volume             |";
      cout.width(11);  cout << cel_vol                       << "|";
      cout.width(11);  cout << map_vol                       << "|\n";

      cout  << "   MAPIN => ----------------------------------------------\n";

      cout.flush();

      }

   // ************** CHECK TO SEE IF MAP SIZES ARE THE SAME ****************

   else
      {
      if (   (X_LIM != MAP_H[map1].NC) ||
             (Y_LIM != MAP_H[map1].NR) ||
             (Z_LIM != MAP_H[map1].NS)     )
         {
         cout
            << "   MAPIN => MAP SIZES DO NOT MATCH !!!\n"
            << "   MAPIN =>          CORRECT     CURRENT\n"
            << "   MAPIN => COLUMNS  ";

         cout.width(7); cout << X_LIM << "     ";
         cout.width(7); cout << MAP_H[map1].NC << "\n   MAPIN => ROWS     ";

         cout.width(7); cout << Y_LIM << "     ";
         cout.width(7); cout << MAP_H[map1].NR << "\n   MAPIN => SECTIONS ";

         cout.width(7); cout << Z_LIM << "     ";
         cout.width(7); cout << MAP_H[map1].NS  << "\n";

         return 2;
         }
      }

   // ************************** LOAD MAP **********************************
   
   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            fread(&MAP[ ( countx-1)          + 
                        ((county-1)*X_LIM )  + 
                        ((countz-1)*XY_LIM)  +
                        ( map1 * XYZ_LIM)      ],
               sizeof(float), 1, read1);

   cout.unsetf(ios::fixed);
   cout.unsetf(ios::right);

   return 0;

   }

//**************************************************************************
//** READ MASK function:  Read in a mask file and stores it in *MSK       **
//**************************************************************************

float ReadMsk(const char *file, int msk1, int mem)
   {

   int      countz;
   int      county;
   int      countx;

   int      tot   = 0;
   int      sum   = 0;

   float    frac  = 0;

   char     ch;

   FILE     *read1;

   if ((read1 = fopen(file, "rb")) == NULL)        // Read failure
      return -1;

   // ********************   READ MAP HEADER ****************************

   fread( &MAP_H[map_mem + msk1].NC     , sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].NR     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].NS     , sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].MODE   , sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].NCSTART, sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].NRSTART, sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].NSSTART, sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].NX     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].NY     , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].NZ     , sizeof(int) ,    1, read1);

   fread(  MAP_H[map_mem + msk1].CELL   , sizeof(float),   6, read1);

   fread( &MAP_H[map_mem + msk1].MAPC   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].MAPR   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].MAPS   , sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].AMIN   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].AMAX   , sizeof(int) ,    1, read1);
   fread( &MAP_H[map_mem + msk1].AMEAN  , sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].ISPG   , sizeof(int) ,    1, read1);

   fread( &MAP_H[map_mem + msk1].NSY    , sizeof(int) ,    1, read1);

   fread(  MAP_H[map_mem + msk1].REST   , sizeof(float),   32, read1);

   fread(  MAP_H[map_mem + msk1].LAB    , sizeof(char) , LAB_LEN, read1);
   fread(  MAP_H[map_mem + msk1].SYM    , sizeof(char) ,
      MAP_H[map_mem + msk1].NSY, read1);

   // ************** CHECK TO SEE IF MASK SIZE IS CORRECT ******************

   if (   (X_LIM != MAP_H[map_mem + msk1].NC) ||
          (Y_LIM != MAP_H[map_mem + msk1].NR) ||
          (Z_LIM != MAP_H[map_mem + msk1].NS)     )
      {
      cout
         << "   MASKI => MAP SIZES DO NOT MATCH !!!\n"
         << "   MASKI =>          CORRECT     CURRENT\n"
         << "   MASKI => COLUMNS  ";

      cout.width(7); cout << X_LIM << "     ";
      cout.width(7); cout << MAP_H[map_mem + msk1].NC 

                          << "\n   MASKI => ROWS     ";
      cout.width(7); cout << Y_LIM << "     ";
      cout.width(7); cout << MAP_H[map_mem + msk1].NR 

                          << "\n   MASKI => SECTIONS ";
      cout.width(7); cout << Z_LIM << "     ";
      cout.width(7); cout << MAP_H[map_mem + msk1].NS << "\n";

      return -1;
      }

   // ****** IF FIRST CALL TO FUNCTION, ASSIGN MEMORY **********************

   if(mem)
      {

      msk_num_1 = msk1;

      cout << "   MASKI => Attempting to assign memory for " << msk_mem 
           << " masks.\n";

      MSK = new char [(XYZ_LIM * msk_mem) + msk_mem];

      if(!MSK)
         {                                         // Not enough memory
         cout  << "\nINSUFFICIENT MEMORY!!!\n";
         return -1;
         }

      cout << "   MASKI => Memory assigned for " << msk_mem 
           << " masks.\n";

      cout << "   MASKI => Setting all pixels to zero.\n";

      for (countx = 0; countx < (XYZ_LIM * msk_mem + msk_mem); countx ++)
         MSK[countx] = 0;

      cout.flush();

      }

   // ************************** LOAD MASK *********************************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            fread(&ch, sizeof(char), 1, read1);

            MSK[ ( countx-1)          +
                 ((county-1)*X_LIM )  +
                 ((countz-1)*XY_LIM)  +
                 ( msk1 * XYZ_LIM)      ] = ch;

            sum = sum + ch;
            tot ++;
            }

   cout  << "   MASKI => Total pixels in mask are " << tot << "\n";
   cout  << "   MASKI => Pixels with value 1 =>   " << sum << "\n";
   cout  << "   MASKI => Pixels with value 0 =>   " << (tot - sum) << "\n";

   frac = ((sum * 1.0) / (tot * 1.0));

   return frac;

   }

//**************************************************************************
//** READ PDB FILE function:  Reads a pdb file and stores it in *PDB      **
//**************************************************************************

int   ReadPDB(const char *file, int pdb1)
   {

   register int   count1;
   register int   count2;

   register int   LOC;

   char  temp[100];

   ifstream read1 (file);

   if (!read1) return 1;

   pdb_len[pdb1] = 0;

   while(!(read1.eof()))         // Until end of file, read atom id's
      {

      read1 >> temp;

      if (   (strcmp(temp,   "ATOM")) &&
             (strcmp(temp, "HETATM"))    )
            {
            read1.getline(temp, 100);
            continue;
            }

      pdb_len[pdb1] ++;

      LOC = (pdb_mem * pdb1) + pdb_len[pdb1];

      read1 >> PDB[LOC].Num;

      read1.getline (PDB[LOC].Nam, 6);
      read1.getline (PDB[LOC].MID, 15);

      read1 >> PDB[LOC].x;
      read1 >> PDB[LOC].y;
      read1 >> PDB[LOC].z;

      read1 >> PDB[LOC].O;
      read1 >> PDB[LOC].B;

      read1.getline (PDB[LOC].END, 20);

      }

   for (count1 = 1; count1 <= pdb_len[pdb1]; count1 ++)
      {
      PDB[count1].Type = 0;
      PDB[count1].Enum = 0;

      LOC = (pdb_mem * pdb1) + count1;

      PDB[LOC].X = C2C_X( 1                             ,
                          PDB[LOC].x + MAP_H[0].CELL[0] , 
                          PDB[LOC].y + MAP_H[0].CELL[1] ,
                          PDB[LOC].z + MAP_H[0].CELL[2] ,
                          MAP_H[0].CELL[0]              ,
                          MAP_H[0].CELL[1]              ,
                          MAP_H[0].CELL[2]              ,
                          MAP_H[0].CELL[3]*DegToRad     ,
                          MAP_H[0].CELL[4]*DegToRad     ,
                          MAP_H[0].CELL[5]*DegToRad        )
                   - X_CELL;

      PDB[LOC].Y = C2C_Y( 1                             ,
                          PDB[LOC].x + MAP_H[0].CELL[0] , 
                          PDB[LOC].y + MAP_H[0].CELL[1] ,
                          PDB[LOC].z + MAP_H[0].CELL[2] ,
                          MAP_H[0].CELL[0]              ,
                          MAP_H[0].CELL[1]              ,
                          MAP_H[0].CELL[2]              ,
                          MAP_H[0].CELL[3]*DegToRad     ,
                          MAP_H[0].CELL[4]*DegToRad     ,
                          MAP_H[0].CELL[5]*DegToRad        )
                   - Y_CELL;

      PDB[LOC].Z = C2C_Z( 1                             ,
                          PDB[LOC].x + MAP_H[0].CELL[0] , 
                          PDB[LOC].y + MAP_H[0].CELL[1] ,
                          PDB[LOC].z + MAP_H[0].CELL[2] ,
                          MAP_H[0].CELL[0]              ,
                          MAP_H[0].CELL[1]              ,
                          MAP_H[0].CELL[2]              ,
                          MAP_H[0].CELL[3]*DegToRad     ,
                          MAP_H[0].CELL[4]*DegToRad     ,
                          MAP_H[0].CELL[5]*DegToRad        )
                   - Z_CELL;

      for (count2 = 1; count2 <= DatNum; count2 ++)
         if (Same(PDBdat[count2].name, PDB[LOC].Nam))
            PDB[LOC].Type = count2;

      }

   return 0;

   }

//**************************************************************************
//** SAME function:  Compares two text files for equality                 **
//**************************************************************************

int Same(char *file1, char *file2)
   {

   int   count1;
   int   count2;

   int   start1;
   int   start2;

   for (count1 = strlen(file1); count1 >= 0; count1 --)
      if ((file1[count1] > 47) && (file1[count1] < 123))
         start1 = count1;

   for (count2 = strlen(file2); count2 >= 0; count2 --)
      if ((file2[count2] > 47) && (file2[count2] < 123))
         start2 = count2;

   for (count1 = start1; count1 < strlen(file1); count1 ++)
      for (count2 = start2; count2 < strlen (file2); count2 ++)
         if ((file1[count1] > 47) && (file1[count1] < 123) &&
             (file2[count2] > 47) && (file2[count2] < 123) &&
             (file1[count1] != file2[count2])                 )
            return 0;

   return 1;

   }

//**************************************************************************
//** READ PDB DATA function:  Reads the pdb file data information         **
//**************************************************************************

int   ReadDATA(const char *file)
   {

   ifstream read1 (file);

   if (!read1) return 1;

   while(!(read1.eof()))         // Until end of file, read atom id's
      {
      DatNum ++;
      read1 >> PDBdat[DatNum].name;
      read1 >> PDBdat[DatNum].e;
      read1 >> PDBdat[DatNum].r;
      }

   DatNum --;

   read1.close();

   return 0;

   }

//**************************************************************************
//** WRITE DATA FILE function:  Writes all known atomic parameters.       **
//**************************************************************************

int   WriteDAT(const char *file, int pdb1)
   {

   register int   LOC;

   register int   count1;

   ofstream write1 (file);

   if (!write1) return 1;

   write1.setf(ios::fixed);
   write1.setf(ios::right);

   write1.precision(2);

   for (count1 = 1; count1 <= pdb_len[pdb1]; count1 ++)
      {

      LOC = (pdb_mem * pdb1) + count1;

      write1.width(5);   write1 << PDB[LOC].Num;
      write1.width(5);   write1 << PDB[LOC].Nam;
      write1.width(14);  write1 << PDB[LOC].MID;

      write1.width(7);   write1 << PDB[LOC].x;
      write1.width(7);   write1 << PDB[LOC].y;
      write1.width(7);   write1 << PDB[LOC].z;

      write1.width(6);   write1 << PDB[LOC].O;
      write1.width(6);   write1 << PDB[LOC].B;

      write1.width(4);   write1 << PDB[LOC].X;
      write1.width(4);   write1 << PDB[LOC].Y;
      write1.width(4);   write1 << PDB[LOC].Z;

      write1.width(3);   write1 << PDB[LOC].Type;

      write1.width(6);   write1 << PDB[LOC].Enum << "\n";

      }

   write1.unsetf(ios::fixed);
   write1.unsetf(ios::right);

   return 0;

   }

//**************************************************************************
//** WRITE PDB FILE function:  Writes a pdb file to storage.              **
//**************************************************************************

int   WritePDB(const char *file, int pdb1)
   {

   register int   LOC;

   register int   count1;

   ofstream write1 (file);

   if (!write1) return 1;

   write1.setf(ios::fixed);
   write1.setf(ios::right);

   for (count1 = 1; count1 <= pdb_len[pdb1]; count1 ++)
      {

      LOC = (pdb_mem * pdb1) + count1;

      write1 << "ATOM  ";

      write1.width(5);   write1 << PDB[LOC].Num;
      write1.width(5);   write1 << PDB[LOC].Nam;
      write1.width(14);  write1 << PDB[LOC].MID;

      write1.precision(3);

      write1.width(8);   write1 << PDB[LOC].x;
      write1.width(8);   write1 << PDB[LOC].y;
      write1.width(8);   write1 << PDB[LOC].z;

      write1.precision(2);

      write1.width(6);   write1 << PDB[LOC].O;
      write1.width(6);   write1 << PDB[LOC].B;

      write1 << PDB[LOC].END << "\n";

      }

   write1.unsetf(ios::fixed);
   write1.unsetf(ios::right);

   return 0;

   }

//**************************************************************************
//** WRITE MAP function:  Writes the current difference map to file       **
//**************************************************************************

int   WriteMap(const char *file, int map1)
   {

   int   countz;
   int   county;
   int   countx;

   float f1;
   float frac_vol;

   FILE  *write1;

   if ((write1 = fopen(file, "wb")) == NULL)        // Write failure
      return -1;


   // ********************  WRITE MAP HEADER ****************************

   fwrite( &MAP_H[0].NC     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].NR     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].NS     , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[0].MODE   , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[0].NCSTART, sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].NRSTART, sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].NSSTART, sizeof(int) ,    1, write1);

   fwrite( &MAP_H[0].NX     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].NY     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].NZ     , sizeof(int) ,    1, write1);

   fwrite(  MAP_H[0].CELL   , sizeof(float),   6, write1);

   fwrite( &MAP_H[0].MAPC   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].MAPR   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].MAPS   , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[0].AMIN   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].AMAX   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[0].AMEAN  , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[0].ISPG   , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[0].NSY    , sizeof(int) ,    1, write1);

   fwrite(  MAP_H[0].REST   , sizeof(float),   32, write1);

   fwrite(  MAP_H[0].LAB    , sizeof(char) , LAB_LEN, write1);
   fwrite(  MAP_H[0].SYM    , sizeof(char) , MAP_H[0].NSY, write1);


   // ************************* WRITE MAP **********************************
 
   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            f1=  (MAP[ ( countx-1)          +
                       ((county-1)*X_LIM )  +
                       ((countz-1)*XY_LIM)  +
                       ( map1 * XYZ_LIM)      ] ) ;
            fwrite(&f1, sizeof(float), 1, write1);
            }

   return 0;

   }

//**************************************************************************
//** WRITE MASK function:  Writes a mask to file.                         **
//**************************************************************************

float MaskOut(const char *file, int msk1)
   {

   int      countz;
   int      county;
   int      countx;

   int      tot   = 0;
   int      sum   = 0;

   float    frac  = 0;

   char     ch;

   FILE     *write1;

   if ((write1 = fopen(file, "wb")) == NULL)        // Write failure
      return -1;

   // ********************  WRITE MASK HEADER ***************************
  
   fwrite( &MAP_H[map_mem + 0].NC     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].NR     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].NS     , sizeof(int) ,    1, write1);
 
   fwrite( &MAP_H[map_mem + 0].MODE   , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[map_mem + 0].NCSTART, sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].NRSTART, sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].NSSTART, sizeof(int) ,    1, write1);

   fwrite( &MAP_H[map_mem + 0].NX     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].NY     , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].NZ     , sizeof(int) ,    1, write1);

   fwrite(  MAP_H[map_mem + 0].CELL   , sizeof(float),   6, write1);
 
   fwrite( &MAP_H[map_mem + 0].MAPC   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].MAPR   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].MAPS   , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[map_mem + 0].AMIN   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].AMAX   , sizeof(int) ,    1, write1);
   fwrite( &MAP_H[map_mem + 0].AMEAN  , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[map_mem + 0].ISPG   , sizeof(int) ,    1, write1);

   fwrite( &MAP_H[map_mem + 0].NSY    , sizeof(int) ,    1, write1);

   fwrite(  MAP_H[map_mem + 0].REST   , sizeof(float),   32, write1);

   fwrite(  MAP_H[map_mem + 0].LAB    , sizeof(char) , LAB_LEN, write1);
   fwrite(  MAP_H[map_mem + 0].SYM    , sizeof(char) , MAP_H[map_mem + 0].NSY, write1);

   // ************************** LOAD MASK *********************************
 
   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {

            ch = MSK[ ( countx-1)          +
                      ((county-1)*X_LIM )  +
                      ((countz-1)*XY_LIM)  +
                      ( msk1 * XYZ_LIM)      ];

            fwrite(&ch, sizeof(char), 1, write1);

            sum = sum + ch;
            tot ++;
            }

   cout  << "   MASKO => Total pixels in mask are " << tot << "\n";
   cout  << "   MASKO => Pixels with value 1 =>   " << sum << "\n";
   cout  << "   MASKO => Pixels with value 0 =>   " << (tot - sum) << "\n";

   frac = ((sum * 1.0) / (tot * 1.0));

   return frac;

   }

//**************************************************************************
//** DISPLAY EVERYTHING TO GRAYSCALE                                      **
//**************************************************************************
 
int   MakeGray(const char *file, int map1)
   {

   register int   mapN[12];
   register float zero[12];
   register float step[12];

   register int   countz;
   register int   county;
   register int   countx;
   register int   map;

   register int   LOC;

   register int   val;

   register char  ch;

   register int   X1;
   register int   X2;

   register int   Y1;
   register int   Y2;

   register int   Z1;
   register int   Z2;

   for (map = 0; map <= map1; map ++)
      {
      cout  << "   GRAY  => Map memory location for file "
            << (map+1) << " (1 to " << map_mem << ")? ";
      cin   >> mapN[map];   mapN[map] --;
      cout  << "   GRAY  => Density begin (-100 = auto) for file "
            << (map+1) << "? ";
      cin   >> zero[map];
      cout  << "   GRAY  => Density step size (-100 = auto) for file "
            << (map+1) << "? ";
      cin   >> step[map];
      }

   cout  << "   GRAY  => Begin X (ROWS) output at (-1 for first)?     ";
   cin   >> X1;   if (X1 == -1) X1 = 1;
   cout  << "   GRAY  => Stop  X (ROWS) output at (-1 for last)?      ";
   cin   >> X2;   if (X2 == -1) X2 = X_LIM;

   cout  << "   GRAY  => Begin Y (COLUMNS) output at (-1 for first)?  ";
   cin   >> Y1;   if (Y1 == -1) Y1 = 1;
   cout  << "   GRAY  => Stop  X (COLUMNS) output at (-1 for last)?   ";
   cin   >> Y2;   if (Y2 == -1) Y2 = Y_LIM;

   cout  << "   GRAY  => Begin Y (SECTIONS) output at (-1 for first)? ";
   cin   >> Z1;   if (Z1 == -1) Z1 = 1;
   cout  << "   GRAY  => Stop  X (SECTIONS) output at (-1 for last)?  ";
   cin   >> Z2;   if (Z2 == -1) Z2 = Z_LIM;

   cout  << "   GRAY  =>\n"
         << "   GRAY  => DISPLAY GRAYSCALE:\n"
         << "   GRAY  =>              BEGIN     END   TOTAL\n";

   cout  << "   GRAY  => ROWS      ";
   cout.width(8);   cout  << X1;
   cout.width(8);   cout  << X2;
   cout.width(8);   cout  << (X2-X1+1) << "\n";

   cout  << "   GRAY  => COLUMNS   ";
   cout.width(8);   cout  << Y1;
   cout.width(8);   cout  << Y2;
   cout.width(8);   cout  << (Y2-Y1+1) << "\n";

   cout  << "   GRAY  => SECTIONS  ";
   cout.width(8);   cout  << Z1;
   cout.width(8);   cout  << Z2;
   cout.width(8);   cout  << (Z2-Z1+1) << "\n   GRAY  =>\n";

   // GRAYSCALE IMMAGE PARAMETERS:
   //    zero => The map value (float) which will correspond to zero
   //    step => The size of each gray ("1") in terms of map densities

   ofstream write1 (file);

   if (!write1)   {  write1.close();   return 1;   }

   for (map = 0; map <= map1; map ++)
      {
      if (zero[map] < -99)
         zero[map] = map_min[map][2];

      if (step[map] < -99)
         step[map] = ((map_max[map][2]-map_min[map][2])/255);

      cout  << "   GRAY  => Gray Parameters for Map # " << (map+1) << "\n"
            << "   GRAY  =>    Output map = " << (mapN[map]+1) << "\n"
            << "   GRAY  =>    Map Zero   = " << zero[map]     << "\n"
            << "   GRAY  =>    Step Size  = " << step[map]     << "\n";
      }

   cout.flush();

   for (countz = Z1; countz <= Z2; countz ++)
      {

      ch = 0;
      for (val = 1; val <= (((X2-X1+2) * (map1+1)) + 1); val ++)
         write1 << ch;

      for (county = Y1; county <= Y2; county ++)
         {
         
         for (map = 0; map <= map1; map ++)
            {

            ch = 0;
            write1 << ch;

            for (countx = X1; countx <= X2; countx ++)
               {

               LOC = ((countx - 1)          ) +
                     ((county - 1) * X_LIM  ) +
                     ((countz - 1) * XY_LIM ) +
                     ( mapN[map]   * XYZ_LIM)   ;

               val = ((MAP[LOC] - zero[map])/step[map]);

               if (val < 0     ) val = 0;
               if (val > 255   ) val = 255;
               ch = val;
               write1 << ch;

               }

            }

         ch = 0;
         write1 << ch;

         }

      }

   ch = 0;
   for (val = 1; val <= (((X2-X1+2) * (map1+1)) + 1); val ++)
      write1 << ch;

   write1.close();

   cout  << "   GRAY  => Done exporting map.\n";

   cout  << "   GRAY  => TYPE THE FOLLOWING TO SEE THE MAP:\n"
         << "   GRAY  => display -geometry " 
         << ((X2-X1+1) * (map1+1) + (map1+1) + 1)
         << "x"  << (((Y2-Y1+2) * (Z2-Z1+1)) + 1)
         << " "  << file << "\n";

   cout.flush();

   return 0;

   }

//**************************************************************************
//** DISPLAY MAP HEADER function:  Displays the map header.               **
//**************************************************************************

void  MapHead(int map1)
   {

   int   len = strlen(MAP_H[map1].LAB);
   int   count1;

   char  string[15];

   while (  (!isalpha(MAP_H[map1].LAB[len]))    &&
            (!isdigit(MAP_H[map1].LAB[len]))    && len   )
      len --;

   if (map1 < map_mem) strcpy (string, "   MAPIN => ");
   else                strcpy (string, "   MASKI => ");

   cout << string << "\n" << string << "Map Label:  ";

   for (count1 = 0; count1 <= len; count1 ++)
      cout  << MAP_H[map1].LAB[count1];

   cout << "\n";

   cout.precision(4);

   cout  << string << "\n"
         << string << "MODE:                 " << MAP_H[map1].MODE << "\n"
         << string << "\n"
         << string << "Columns   (X grid):   " << MAP_H[map1].NC   << "\n"
         << string << "Rows      (Y grid):   " << MAP_H[map1].NR   << "\n"
         << string << "Sections  (Z grid):   " << MAP_H[map1].NS   << "\n"
         << string << "\n"
         << string << "First column:         " << MAP_H[map1].NCSTART << "\n"
         << string << "First row:            " << MAP_H[map1].NRSTART << "\n"
         << string << "First section:        " << MAP_H[map1].NSSTART << "\n"
         << string << "\n"
         << string << "Axis order:           " << MAP_H[map1].MAPC << " "
                                               << MAP_H[map1].MAPR << " "
                                               << MAP_H[map1].MAPS << "\n"
         << string << "\n"
         << string << "Space group number:   " << MAP_H[map1].ISPG << "\n"
         << string << "\n"
         << string << "Unit cell:\n"
         << string << "   X (A)                 " << MAP_H[map1].CELL[0]<< "\n"
         << string << "   Y (A)                 " << MAP_H[map1].CELL[1]<< "\n"
         << string << "   Z (A)                 " << MAP_H[map1].CELL[2]<< "\n"
         << string << "   Alpha                 " << MAP_H[map1].CELL[3]<< "\n"
         << string << "   Beta                  " << MAP_H[map1].CELL[4]<< "\n"
         << string << "   Gamma                 " << MAP_H[map1].CELL[5]<< "\n"
         << string << "\n"
         << string << "   X Sections            " << MAP_H[map1].NX << "\n"
         << string << "   Y Sections            " << MAP_H[map1].NY << "\n"
         << string << "   Z Sections            " << MAP_H[map1].NZ << "\n"
         << string << "\n"
         << string << "Electron Density:\n"
         << string << "   Minimum               " << MAP_H[map1].AMIN     << "\n"
         << string << "   Maximum               " << MAP_H[map1].AMAX     << "\n"
         << string << "   Average               " << MAP_H[map1].AMEAN    << "\n"
         << string << "   RMS Deviation         " << MAP_H[map1].REST[30] << "\n"
         << string << "\n";

   cout.flush();

   return;

   }

//**************************************************************************
//** SCALE function:  Scales two maps together inside or outside of mask  **
//**************************************************************************

float Scale(int map1, int map2, int zone, int msk1)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register float scale;

   // ****************** SCALE MAP1 TO MAP2 INSIDE MSK1 ********************

   cout << "   SCALE => Average for map 1 is "   // Find max,min,tot,avg
        << FindParms(map1, zone, msk1) << "\n";  //    inside MASK region
 
   cout << "   SCALE => Average for map 2 is "   // Find max,min,tot,avg
        << FindParms(map2, zone, msk1) << "\n";  //    inside MASK region

   scale = (map_avg[map2][zone]/map_avg[map1][zone]);
                                                   // Scale factor

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM ) +
                  ( map1        * XYZ_LIM)   ;

            MAP[LOC] = MAP[LOC] * scale;
            }

   return scale;

   }

//**************************************************************************
//** RFAC function:  Finds R factor between two maps in/out of a mask.    **
//**************************************************************************

float Rfac(int map1, int map2, int zone, int msk1, int type)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register float  num;

   register double value = 0;

   register float zone2;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   // ********** FIND RFACTOR BETWEEN MAP1, MAP2 IN/OUT OF MSK1 ************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (zone2 != 2) &&
                 (MSK[LOC + (msk1 * XYZ_LIM)] == zone2) )
               continue;

            num = MAP[LOC + (map1 * XYZ_LIM)] -
                  MAP[LOC + (map2 * XYZ_LIM)];

            value = value + sqrt (num * num);
            }

   cout  << "   RFAC  => -----------------------------------------------\n";
   cout  << "   RFAC  => | Average density for map 1    | "; 
   cout.width(12);
   cout  << FindParms(map1, zone, msk1)
         << " |\n";
   cout  << "   RFAC  => | Average density for map 2    | "; 
   cout.width(12);
   cout  << FindParms(map2, zone, msk1)
         << " |\n";
   cout  << "   RFAC  => | RMS value for map 1          | "; 
   cout.width(12);
   cout  << FindRMS(map1, zone, msk1)
         << " |\n";
   cout  << "   RFAC  => | RMS value for map 2          | "; 
   cout.width(12);
   cout  << FindRMS(map2, zone, msk1)
         << " |\n";
   cout  << "   RFAC  => | Sum of map pixel differences | "; 
   cout.width(12);
   cout  << value 
         << " |\n";
   cout  << "   RFAC  => | Number of pixels in zone     | "; 
   cout.width(12);
   cout  << map_num[map2][zone]
         << " |\n";
   cout  << "   RFAC  => | Average difference per pixel | "; 
   cout.width(12);
   cout  << (value/map_num[map1][zone]) 
         << " |\n";
   cout  << "   RFAC  => | Avg diff / Map 1 average     | "; 
   cout.width(12);
   cout  << ((value/map_num[map1][zone])/map_avg[map1][zone])
         << " |\n";
   cout  << "   RFAC  => | Avg diff / Map 2 average     | "; 
   cout.width(12);
   cout  << ((value/map_num[map1][zone])/map_avg[map2][zone])
         << " |\n";
   cout  << "   RFAC  => | Avg diff / ((Avg1 + Avg2)/2) | "; 
   cout.width(12);
   cout  << ( (value/map_num[map1][zone])/
              ((map_avg[map1][zone]+map_avg[map2][zone])/2) )
         << " |\n";
   cout  << "   RFAC  => | Avg diff / Map 1 RMS         | "; 
   cout.width(12);
   cout  << ((value/map_num[map1][zone])/map_rms[map1][zone])
         << " |\n";
   cout  << "   RFAC  => | Avg diff / Map 2 RMS         | "; 
   cout.width(12);
   cout  << ((value/map_num[map1][zone])/map_rms[map2][zone])
         << " |\n";
   cout  << "   RFAC  => | Avg diff / ((RMS1 + RMS2)/2) | "; 
   cout.width(12);
   cout  << ( (value/map_num[map1][zone])/
              ((map_rms[map1][zone]+map_rms[map2][zone])/2) )
         << " |\n";
   cout  << "   RFAC  => -----------------------------------------------\n";

   switch (type)
      {
      case 1:
         value = ((value/map_num[map1][zone])/map_avg[map1][zone]);
         break;
      case 2:
         value = ((value/map_num[map1][zone])/map_avg[map2][zone]);
         break;
      case 3:
         value = ( (value/map_num[map1][zone])/
                   ((map_avg[map1][zone]+map_avg[map2][zone])/2) );
         break;
      case 4:
         value = ((value/map_num[map1][zone])/map_rms[map1][zone]);
         break;
      case 5:
         value = ((value/map_num[map1][zone])/map_rms[map2][zone]);
         break;
      case 6:
          value = ( (value/map_num[map1][zone])/
                  ((map_rms[map1][zone]+map_rms[map2][zone])/2) );
         break;
      default:
         value = (value/map_num[map1][zone]);
      }

   return value;

   }


//**************************************************************************
//** SMEAR function: Smooths map1 by and saves in map2 (map3 is temp.)    **
//**************************************************************************

void  Smear(int map1, int map2, int map3, int N)
   {

   register int   z2;
   register int   y2;
   register int   x2;

   register int   z3;
   register int   y3;
   register int   x3;

   register int   z3a;
   register int   y3a;
   register int   x3a;

   register int   LOC2;
   register int   LOC3;

   register int   dx;
   register int   dy;
   register int   dz;

   register float X = 0;
   register float den;

   register float mod[20];

   for (x2 = 1; x2 < N; x2 ++)
      X = X + x2;
   X = X * 2;
   X = X + N;

   cout  << "   SMEAR => Pixel multiplication table:\n";

   for (x2 = 0; x2 < N; x2 ++)
      {
      mod[x2] = (N - x2) / X;
      cout  << "   SMEAR => " << x2 << "\t" << mod[x2] << "\n";
      }

   // ********** SMOOTH MAP1 BY SMEARING TO N PIXELS IN EACH DIRECTION *****

   cout  << "   SMEAR => Copying maps and setting to zero.\n";

   x2 = Zero(map2, 2, 0);
   MapMod(map2, map1, 2, 0, +1);
   x2 = Zero(map3, 2, 0);

   cout  << "   SMEAR => SMOOTHING MAP IN X DIRECTION.\n";
   cout.flush();

   for (z2 = 1; z2 <= Z_LIM; z2 ++)
      for (y2 = 1; y2 <= Y_LIM; y2 ++)
         for (x2 = 1; x2 <= X_LIM; x2 ++)
            {

            LOC2 = ((x2 - 1)          ) +
                   ((y2 - 1) * X_LIM  ) +
                   ((z2 - 1) * XY_LIM ) +
                   (XYZ_LIM * map2);

            den = MAP[LOC2];

            for (x3a = (x2 - N + 1); x3a < (x2 + N); x3a ++)
               {

               dx = Abs(x2-x3a) + .01;

                    if (x3a < 0    ) x3 = x3a + X_LIM;
               else if (x3a > X_LIM) x3 = x3a - X_LIM;
               else x3 = x3a;

               LOC3 = ((x3 - 1)          ) +
                      ((y2 - 1) * X_LIM  ) +
                      ((z2 - 1) * XY_LIM ) +
                      (XYZ_LIM * map3);

               MAP[LOC3] = MAP[LOC3] + (den * mod[dx]);
                                     
               }

            }

   cout  << "   SMEAR => Copying maps and setting to zero.\n";

   x2 = Zero(map2, 2, 0);
   MapMod(map2, map3, 2, 0, +1);
   x2 = Zero(map3, 2, 0);

   cout  << "   SMEAR => SMOOTHING MAP IN Y DIRECTION.\n";
   cout.flush();

   for (z2 = 1; z2 <= Z_LIM; z2 ++)
      for (y2 = 1; y2 <= Y_LIM; y2 ++)
         for (x2 = 1; x2 <= X_LIM; x2 ++)
            {

            LOC2 = ((x2 - 1)          ) +
                   ((y2 - 1) * X_LIM  ) +
                   ((z2 - 1) * XY_LIM ) +
                   (XYZ_LIM * map2);

            den = MAP[LOC2];

            for (y3a = (y2 - N + 1); y3a < (y2 + N); y3a ++)
               {

               dy = Abs(y2-y3a) + .01;

                    if (y3a < 0    ) y3 = y3a + Y_LIM;
               else if (y3a > Y_LIM) y3 = y3a - Y_LIM;
               else y3 = y3a;

               LOC3 = ((x2 - 1)          ) +
                      ((y3 - 1) * X_LIM  ) +
                      ((z2 - 1) * XY_LIM ) +
                      (XYZ_LIM * map3);

               MAP[LOC3] = MAP[LOC3] + (den * mod[dy]);

               }

            }

   cout  << "   SMEAR => Copying maps and setting to zero.\n";

   x2 = Zero(map2, 2, 0);
   MapMod(map2, map3, 2, 0, +1);
   x2 = Zero(map3, 2, 0);

   cout  << "   SMEAR => SMOOTHING MAP IN Z DIRECTION.\n";
   cout.flush();

   for (z2 = 1; z2 <= Z_LIM; z2 ++)
      for (y2 = 1; y2 <= Y_LIM; y2 ++)
         for (x2 = 1; x2 <= X_LIM; x2 ++)
            {

            LOC2 = ((x2 - 1)          ) +
                   ((y2 - 1) * X_LIM  ) +
                   ((z2 - 1) * XY_LIM ) +
                   (XYZ_LIM * map2);

            den = MAP[LOC2];

            for (z3a = (z2 - N + 1); z3a < (z2 + N); z3a ++)
               {

               dz = Abs(z2-z3a) + .01;

                    if (z3a < 0    ) z3 = z3a + Z_LIM;
               else if (z3a > Z_LIM) z3 = z3a - Z_LIM;
               else z3 = z3a;

               LOC3 = ((x2 - 1)          ) +
                      ((y2 - 1) * X_LIM  ) +
                      ((z3 - 1) * XY_LIM ) +
                      (XYZ_LIM * map3);

               MAP[LOC3] = MAP[LOC3] + (den * mod[dz]);

               }

            }

   x2 = Zero(map2, 2, 0);
   MapMod(map2, map3, 2, 0, +1);
   x2 = Zero(map3, 2, 0);

   return;

   }

//**************************************************************************
//** ROUGH function: Calculates map roughness in map2                     **
//**************************************************************************

void  Rough(int map1, int map2, int N)
   {

   register int   z1;
   register int   y1;
   register int   x1;

   register int   z1a;
   register int   y1a;
   register int   x1a;

   register int   z2;
   register int   y2;
   register int   x2;

   register int   LOC1;
   register int   LOC2;

   register int   dx;
   register int   dy;
   register int   dz;

   register float den;
   register float del;

   register float mod[20];

   register float val[10000];
   register int   num;
   register float avg;
   register float rms;
   register float min_rough = 100;
   register float max_rough = 0;

   register int temp =0;

   if (N > 10) N = 10;

   // ********** CALCULATE ROUGHNESS BY ADDING RMS IN EACH DIRECTION *****

//   cout  << "   ROUGH => Setting map to zero. \n";

//   x2 = Zero(map2, 2, 0);

   cout  << "   ROUGH => CALCULATING ROUGHNESS OF MAP " << (map1+1) << " RADIUS " << N << " TO MAP " << (map2+1) << " ...\n";
   cout.flush();

   for (z2 = 1; z2 <= Z_LIM; z2 ++)
      for (y2 = 1; y2 <= Y_LIM; y2 ++)
         for (x2 = 1; x2 <= X_LIM; x2 ++)
            {

            LOC2 = ((x2 - 1)          ) +
                   ((y2 - 1) * X_LIM  ) +
                   ((z2 - 1) * XY_LIM ) +
                   (XYZ_LIM * map2);

	    num = 0;
	    avg = 0;
	    rms = 0;
//cout << "*******\n";

            for (x1 = (x2 - N); x1 < (x2 + N); x1 ++)
               for (y1 = (y2 - N); y1 < (y2 + N); y1 ++)
                  for (z1 = (z2 - N); z1 < (z2 + N); z1 ++)
                     {

                     dx = x2-x1;
                     dy = y2-y1;
                     dz = z2-z1;

		     del = sqrt((dx*dx)+(dy*dy)+(dz*dz));

		     if (del > N) continue;

		     x1a = x1;
		     y1a = y1;
		     z1a = z1;

//temp++;
//if (!(temp%1)) cout << del << " ";
                          if (x1a < 1    )  x1a = x1a + X_LIM;
                     else if (x1a > X_LIM)  x1a = x1a - X_LIM;

                          if (y1a < 1    )  y1a = y1a + Y_LIM;
                     else if (y1a > Y_LIM)  y1a = y1a - Y_LIM;

                          if (z1a < 1    )  z1a = z1a + Z_LIM;
                     else if (z1a > Z_LIM)  z1a = z1a - Z_LIM;

                     LOC1 = ((x1a - 1)          ) +
                            ((y1a - 1) * X_LIM  ) +
                            ((z1a - 1) * XY_LIM ) +
                            (XYZ_LIM * map1);
//cout << " X=" << x1a << " Y=" << y1a << " Z=" << z1a << " loc=" << LOC1 << "  ";
//cout.flush();
                     val[num] = MAP[LOC1];
		     avg = avg + val[num];
		     num ++;

		     }

//cout << "\n" << num << "\n";

            avg = avg / num;

	    for (x1 = 0; x1 < num; x1 ++)
	       rms = rms + pow((val[x1] - avg), 2.0);

	    rms = sqrt(rms);

	    MAP[LOC2] = rms;
	    if (min_rough > rms) min_rough = rms;
	    if (max_rough < rms) max_rough = rms;

            }

   cout  << "   ROUGH => Roughness values between " << min_rough << " and " << max_rough 
	 << " from " << map1 << " saved to " << map2 << "\n";

   return;

   }





//**************************************************************************
//** SHAPE function: Expands a mask to inflection points                  **
//**************************************************************************

void  Shape(int msk1, int msk2, int msk3, int map1, float MinDif, int N1, int N2) // Modifies mask by expanding until inflection points in all directions
   {

   register int cycle = 0;

   register int   z2;
   register int   y2;
   register int   x2;

   register int   LOC1;
   register int   LOC2;
   register int   LOC3;

   register int   dx;
   register int   dy;
   register int   dz;

   register int   num;
   register int   ConstrNum = 0;
   register int   ChangeNum = 0;
   register int   ConstrTot = 0;
   register int   ChangeTot = 0;
   register float cur = 0;
   register float low = 0;

   cout  << "   SHAPE => Making mask copy and setting temporary location to zero. \n";

   MskCopy(msk1, msk2);
   MskCopy(msk1, msk3);

   for (cycle = 0; cycle <= N1; cycle ++)
      {
      cout  << "   SHAPE => Expanding mask " << (msk1+1) << " cycle " << cycle << "\n";

      for (z2 = 3; z2 <= (Z_LIM-2); z2 ++)
         for (y2 = 3; y2 <= (Y_LIM-2); y2 ++)
            for (x2 = 3; x2 <= (X_LIM-2); x2 ++)
               {
	       LOC1 = ((x2 - 1)          ) +
                      ((y2 - 1) * X_LIM  ) +
                      ((z2 - 1) * XY_LIM )  ;

               num = 0;
	       cur = MAP [  LOC1 + (XYZ_LIM * map1)        ];
               low = cur;
	       LOC3 = 0;

	       for (dz=(z2-1); dz<=(z2+1); dz++)
	          for (dy=(y2-1); dy<=(y2+1); dy++)
	             for (dx=(x2-1); dx<=(x2+1); dx++)
			{
	                LOC2 = ((dx - 1)          ) +
                               ((dy - 1) * X_LIM  ) +
                               ((dz - 1) * XY_LIM )  ;
  
                        num = num + MSK [ LOC2 + (XYZ_LIM * msk2) ];

			if (( low > MAP [ LOC2 + (XYZ_LIM * map1) ] ) && (!(MSK [ LOC2 + (XYZ_LIM * msk2) ] )))
		           {
			   low = MAP [ LOC2 + (XYZ_LIM * map1) ];
			   LOC3 = LOC2;
			   }
	               }
 
               if      (num < N2)                                  ConstrNum ++;
	       else if ((LOC3 ) && (low < (cur - MinDif) ))
	          {
	          ChangeNum ++; 
		  MSK [ LOC3 + (XYZ_LIM * msk3) ] = 1;
		  }
               }
      cout  << "   SHAPE => Total pixels changed: " << ChangeNum;
      cout  << "   SHAPE => Total constrictions pinched:  " << ConstrNum;

      ChangeTot = ChangeTot + ChangeNum;
      ConstrTot = ConstrTot + ConstrNum;

      MskCopy(msk3, msk2);
      }

   return;

   }



//**************************************************************************
//** ZERO function:  Sets part of all of map to zero.                     **
//**************************************************************************

int   Zero(int map1, int zone, int msk1)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register int   total = 0;

   register float zone2;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   // *********** ASSIGN DENSITY = ZERO INSIDE THE MASK REGION *************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (zone2 != 2) &&
                 (MSK[LOC + (msk1 * XYZ_LIM)] == zone2) )
               continue;

            MAP[LOC + (XYZ_LIM * map1)] = 0;
            total ++;
            }

   return total;

   }
  
//**************************************************************************
//** CUT function:  Sets any value in part or all of map to min/max if    **
//**    it is bellow/above min/max density.                               **
//**************************************************************************

int   Cut(int map1, int zone, int msk1, float min, float max)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register int   total = 0;

   register float zone2;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   // *********** ASSIGN DENSITY = ZERO INSIDE THE MASK REGION *************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (zone2 != 2) &&
                 (MSK[LOC + (msk1 * XYZ_LIM)] == zone2) )
               continue;

            if (MAP[LOC + (XYZ_LIM * map1)] < min)
               {
               MAP[LOC + (XYZ_LIM * map1)] = min;
               total ++;
               }

            if (MAP[LOC + (XYZ_LIM * map1)] > max)
               {
               MAP[LOC + (XYZ_LIM * map1)] = max;
               total ++;
               }

            }

   return total;

   }

//**************************************************************************
//** FIND MAXIMUM VALUE of two maps and save in third map location.       **
//**************************************************************************

void MaxOf(int map1, int map2, int map3)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register float val1;
   register float val2;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            val1 = MAP[LOC + (XYZ_LIM * map2)];
            val2 = MAP[LOC + (XYZ_LIM * map3)];

            if (val1 < val2) val1 = val2;

            MAP[LOC + (XYZ_LIM * map1)] = val1;

            }

   return;

   }

//**************************************************************************
//** FIND MAXIMUM VALUE of two masks and saves in thirs mask location.    **
//**************************************************************************

void  MaxMs(int msk1, int msk2, int msk3)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (MSK[LOC + (XYZ_LIM * msk2)]) || 
                 (MSK[LOC + (XYZ_LIM * msk3)])    )
               MSK[LOC + (XYZ_LIM * msk1)] = 1;

            else
               MSK[LOC + (XYZ_LIM * msk1)] = 0;
            }

   return;

   }

//**************************************************************************
//** FIND MINIMUM VALUE of two masks and saves in thirs mask location.    **
//**************************************************************************

void  MinMs(int msk1, int msk2, int msk3)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (MSK[LOC + (XYZ_LIM * msk2)] == 0) || 
                 (MSK[LOC + (XYZ_LIM * msk3)] == 0)    )
               MSK[LOC + (XYZ_LIM * msk1)] = 0;

            else
               MSK[LOC + (XYZ_LIM * msk1)] = 1;
            }

   return;

   }

//**************************************************************************
//** FLIP 1 AND 0 IN A MASK to make a new mask.                           **
//**************************************************************************

void  Flip(int msk1, int msk2)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if (MSK[LOC + (XYZ_LIM * msk2)])
                MSK[LOC + (XYZ_LIM * msk1)] = 0;

            else
                MSK[LOC + (XYZ_LIM * msk1)] = 1;
            }

   return;

   }

//**************************************************************************
//** COPY MASK FILE                                                       **
//**************************************************************************

void MskCopy(int msk1, int msk2)     // Copy msk1 into msk2
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            MSK[LOC + (XYZ_LIM * msk2)] = MSK[LOC + (XYZ_LIM * msk1)];
            }

   return;

   }


//**************************************************************************
//** COPY MAP  FILE                                                       **
//**************************************************************************

void MapCopy(int map1, int map2)     // Copy map1 into map2
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            MAP[LOC + (XYZ_LIM * map2)] = MAP[LOC + (XYZ_LIM * map1)];
            }

   return;

   }


//**************************************************************************
//** ADD/SUBTRACT MAPS function: MAPS function:                           **
//** Adds or subtracts maps inside/outside a mask.                        **
//**************************************************************************

void  MapMod(int map1, int map2, int zone, int msk1, float value)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register int   zone2;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   // **********ADD/SUBTRACT MAP2 TO/FROM MAP1 IN/OUT OF MSK1 **************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (zone2 != 2) &&
                 (MSK[LOC + (msk1 * XYZ_LIM)] == zone2) )
               continue;

            MAP[LOC + (XYZ_LIM * map1)] = MAP[LOC + (XYZ_LIM * map1)]
               + (value *  MAP[LOC + (XYZ_LIM * map2)]);
            }
  
   return;

   }

//**************************************************************************
//** FIND PARAMETERS function: Finds MAXIMUM, MINIMUM, TOTAL, and         **
//**    AVERAGE electron density for map map1 inside/outside of mask msk1 **
//**************************************************************************

float FindParms(int map1, int zone, int msk1)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC1;
   register int   LOC2;

   register float val;

   register int   zone2;

   map_max[map1][zone] = -1000;
   map_min[map1][zone] = +1000;
   map_avg[map1][zone] = 0;
   map_tot[map1][zone] = 0;
   map_num[map1][zone] = 0;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC2 = ((countx - 1)          ) +
                   ((county - 1) * X_LIM  ) +
                   ((countz - 1) * XY_LIM ) +
                   ( msk1   * XYZ_LIM     );

            if ( (zone2 != 2) &&
                 (MSK[LOC2] == zone2) )
               continue;

            LOC1 = ((countx - 1)          ) +
                   ((county - 1) * X_LIM  ) +
                   ((countz - 1) * XY_LIM ) +
                   ( map1   * XYZ_LIM     );

            val = MAP[LOC1];

            if (val > map_max[map1][zone] ) map_max[map1][zone] = val;
            if (val < map_min[map1][zone] ) map_min[map1][zone] = val;

            map_tot[map1][zone] = map_tot[map1][zone] + val;
            map_num[map1][zone] ++;
            }

   map_avg[map1][zone] = map_tot[map1][zone] 
                         / (map_num[map1][zone] * 1.0);
   map_tot[map1][zone] = map_tot[map1][zone] 
                         * map_vol / (XYZ_LIM * 1.0);

   if (zone2 == 2)
      {
      MAP_H[map1].AMAX  = map_max[map1][zone];
      MAP_H[map1].AMIN  = map_min[map1][zone];
      MAP_H[map1].AMEAN = map_avg[map1][zone];
      }

   return map_avg[map1][zone];

   }

//**************************************************************************
//** FIND RMS VARIANCE function:  Finds the density root mean square      **
//**    variance for a map1 inside/outside of msk.                        **
//**************************************************************************

float FindRMS(int map1, int zone, int msk1)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC1;
   register int   LOC2;

   register float sum = 0;

   register int   zone2;

   map_var[map1][zone] = 0;
   map_rms[map1][zone] = 0;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;
   
   // map_var = ((1/N) sum ((density - average)^2)) => Standard Deviation
   // map_rms = sqrt (map_var)

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC2 = ((countx - 1)          ) +
                   ((county - 1) * X_LIM  ) +
                   ((countz - 1) * XY_LIM ) +
                   ( msk1   * XYZ_LIM     );

            if ( (zone2 != 2) &&
                 (MSK[LOC2] == zone2) )
               continue;

            LOC1 = ((countx - 1)          ) +
                   ((county - 1) * X_LIM  ) +
                   ((countz - 1) * XY_LIM ) +
                   ( map1   * XYZ_LIM     );

            sum = sum + (  (MAP[LOC1] - map_avg[map1][zone]) * 
                           (MAP[LOC1] - map_avg[map1][zone])   );

            }

   map_var[map1][zone] = (sum/map_num[map1][zone]); // (1/N) sum((P-Po)^2)
   map_rms[map1][zone] = sqrt(map_var[map1][zone]); // sqrt (var)

   if (zone2 == 2)
      MAP_H[map1].REST[30] = map_rms[map1][zone];

   return map_rms[map1][zone];

   }

//**************************************************************************
//** MAP ADD function:  Adds a constant, or the variable in the memory    **
//**    location, to a map in/out of a mask.                              **
//**************************************************************************

void MapAdd(int map1, int zone, int msk1, float value)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register int   zone2;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   // **********ADD/SUBTRACT CONSTANT FROM MAP1 IN/OUT OF MSK1 *************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (zone2 != 2) &&
                 (MSK[LOC + (msk1 * XYZ_LIM)] == zone2) )
               continue;

            MAP[LOC + (map1 * XYZ_LIM)] =
               MAP[LOC + (map1 * XYZ_LIM)] + value;
            }

   return;

   }


//**************************************************************************
//** MAP ADD function:  Adds a constant, or the variable in the memory    **
//**    location, to a map in/out of a mask.                              **
//**************************************************************************

void MapMult(int map1, int zone, int msk1, float value)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   LOC;

   register int   zone2;

   if (zone == 0) zone2 = 1;
   if (zone == 1) zone2 = 0;
   if (zone == 2) zone2 = 2;

   // ********** MULTIPLY MAP1 BY CONSTANT VALUE IN/OUT OF MSK1 ************

   for (countz = 1; countz <= Z_LIM; countz ++)
      for (county = 1; county <= Y_LIM; county ++)
         for (countx = 1; countx <= X_LIM; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            if ( (zone2 != 2) &&
                 (MSK[LOC + (msk1 * XYZ_LIM)] == zone2) )
               continue;

            MAP[LOC + (map1 * XYZ_LIM)] =
               MAP[LOC + (map1 * XYZ_LIM)] * value;
            }

   return;

   }

//**************************************************************************
//** CELL VOLUME function:  Calculates the unit cell volume               **
//**************************************************************************

float cell_volume(float A, float B, float C, float a, float b, float c)
   {
   return   (2*A*B*C) * pow(  (  (sin((+a+b+c)*PI/360))   *
                                 (sin((-a+b+c)*PI/360))   *
                                 (sin((+a-b+c)*PI/360))   *
                                 (sin((+a+b-c)*PI/360))     ), 0.5);
   }

//**************************************************************************
//** KEYS function:  Displays control keys and arguments in a list.       **
//**************************************************************************

void Keys()
   {
   cout 
   << "   KEYS  => HELP                          KEYS\n"
   << "   KEYS  => LIST                          \n"
   << "   KEYS  =>\n"
   << "   KEYS  => MAPIN X1 'name'               MASKI X2 'name'\n"
   << "   KEYS  => NAME {type}{loc} 'name'\n"
   << "   KEYS  =>\n"
   << "   KEYS  => MAXMS Y1 Y2 Y3                MINMS Y1 Y2 Y3\n"
   << "   KEYS  => FLIP  Y1 Y2\n"
   << "   KEYS  =>\n"
   << "   KEYS  => PDBIN {num}{len} P1 'name'    PDBOU P1 'name'\n"
   << "   KEYS  => PDBDA P1 'name'               OCCUP P1 X1\n" 
   << "   KEYS  =>\n"
   << "   KEYS  => RFAC X1 X2 IN/OUT/TOTAL Y1    RMS X1 IN/OUT/TOTAL Y1\n"
   << "   KEYS  => SMEAR X1 X2 X3 N              MAXOF X1 X2 X3\n"
   << "   KEYS  => SCALE X1 Y1 IN/OUT X2         ZERO X1 IN/OUT X2\n"
   << "   KEYS  => ADD X1 Y1 IN/OUT X2           SUB X1 Y1 IN/OUT X2\n"
   << "   KEYS  => COMB X1 Y1 IN/OUT/TOTAL X2 F  CUT X1 IN/OUT X2 MIN MAX\n"
   << "   KEYS  => AVG X1 IN/OUT                 NEG\n"
   << "   KEYS  => PLUS X1 IN/OUT X2 VALUE/F     MULT X1 IN/OUT/TOTAL X2 F\n"
   << "   KEYS  =>\n"
   << "   KEYS  => WRITE X1 'name'               END, QUIT, STOP, EXIT\n"
   << "   KEYS  =>\n"
   << "   KEYS  => GRAY 'name' n X1 X1Start X1Step ... Xn XnStart XnStep\n"
   << "   KEYS  =>    x1 x2 y1 y2 z1 z2\n";
   return;
   }

//**************************************************************************
//** GET ENTRY function:  Gets an entry from the console (like cin), but  **
//**    won't go beyond the end of the line (returns null if an entry is  **
//**    requested but a line feed is the next character in the stream).   **
//**************************************************************************

char *get_entry()
   {
   int         len = 0;
   char        ch;
   static char input[20] = "";

   while (cin.get(ch))
      {
      if (ch == '\n')
         {
         cin.putback(ch);
         input[len] = '\0';
         return input;
         }

      if (isgraph(ch))
         {
         input[len] = ch;
         len ++;

         ch = cin.peek();
         if (ch == ' ')
            {
            input[len] = '\0';
            return input;
            }
         }
      }
   return 0;
   }

//**************************************************************************
//** UPPER function:  Transforms a string to ALL CAPS.                    **
//**************************************************************************

char  *upper(char input[20])
   {

   static char output[20];
   strcpy(output, input);

   int         count1;
   int         end         = strlen(output) - 1;

   for (count1 = 0; count1 <= end; count1 ++)
      if ((output[count1] >= 97) && (output[count1] <= 122))
         output[count1] = (output[count1] - 32);

   return output;

   }

//**************************************************************************
//** Change a string to its float equivalent, discarding any              **
//** non-numericals (so that 12.5, +12.5, **12.5**, and A12b.5 will all   **
//** return as 12.5).                                                     **
//**************************************************************************

float Ch2float(char input[15])
   {

   int      count1;

   int      number      = 0;
   float    result      = 0;

   int      digits      = 0;
   float    digit[10];

   int      mult        = +1;
   int      dec         = 0;

   for (count1 = 0; (count1 < 15 && input[count1]); count1 ++)
      {
      if ((input[count1] >= 48) && (input[count1] <=57))
         {
         number ++;
         digit[number] = input[count1] - 48;
         }
      else if (input[count1] == '.')   // Keep track of the decimal location
         {
         digits = number;
         dec    = 1;
         }

      else if (input[count1] == '-')   // Negative numbers
         mult = -1;
      }

   if (!dec) digits = number;

   for (count1 = 1; count1 <= number; count1 ++)
      result = result + (  digit[count1]                 *
                           pow(10.0, (digits - count1))     );

   return (result * mult);

   }

//**************************************************************************
//** ZONE FIND function:  Find zone in mask region.                       **
//**************************************************************************

int   zone_find(char input[15])
   {

   char key[10];
   int  zone;

   cout  << "   " << input << "Operate IN/OUT of mask, or TOTAL? ";
   cin   >> key;

        if (key[0] == 'I' || key[0] == 'i') zone = 1;
   else if (key[0] == 'O' || key[0] == 'o') zone = 0;
   else zone = 2;

   return zone;
   }

//**************************************************************************
//** DISTANCE function:  X, Y, Z distance => Returns angstrom distance    **
//** between two points separated by X grid points, Y grid points, and    **
//** Z grid points in the x, y, and z directions.                         **
//**************************************************************************

float distance(int X, int Y, int Z)
   {
   float x1 = ((X * 1.0)/ X_CELL);
   float y1 = ((Y * 1.0)/ Y_CELL);
   float z1 = ((Z * 1.0)/ Z_CELL);

   float x2 = MAP_H[0].CELL[0];
   float y2 = MAP_H[0].CELL[1];
   float z2 = MAP_H[0].CELL[2];

   float a  = MAP_H[0].CELL[3];
   float b  = MAP_H[0].CELL[4];
   float c  = MAP_H[0].CELL[5];

   float answer;

   answer = x1 * x1 * x2 * x2                         +
            y1 * y1 * y2 * y2                         +
            z1 * z1 * z2 * z2                         +
            x1 * y1 * x2 * y2 * 2 * cos(c * PI / 360) +
            x1 * z1 * x2 * z2 * 2 * cos(b * PI / 360) +
            y1 * z1 * y2 * z2 * 2 * cos(a * PI / 360)   ;

   return (sqrt(answer));
   }

//**************************************************************************
//** MINIMUM VALUE function: Finds the smallest of two numbers.           **
//**************************************************************************

float Min(float v1, float v2)
   {
   if (v1 < v2) return v1;
   return v2;
   }

//**************************************************************************
//** ABSOLUTE VALUE function: Finds the absolute value of a float.        **
//**************************************************************************

float Abs(float value)
   {
   return sqrt(value * value);
   }

//**************************************************************************
//** GAUSSIAN function:  Returns value of a gaussian of width B at dist.  **
//**************************************************************************

float Gauss(float S, float X)
   {
   return (  (1/(sqrt(PI)*S)) * (pow(E,-((X*X)/(S*S)))   )   );
   }

//**************************************************************************
//** FIND XYZ COORDINATE FUNCTION: X=(M)x and x=(M-1)X                    **
//** Both X and x are positions vectors in real space, but:               **
//**    X is a position vector in cartesian  coordinates (Angstroms)      **
//**    x is a position vector in fractional coordinates (dimensionless)  **
//**************************************************************************

//**************************************************************************
//** Calculate X in fractional coordinates from X in real coordinates     **
//**************************************************************************

int      C2C_X( int type,
                float X1, float Y1, float Z1,
                float A , float B , float C ,
                float a , float b , float c   )
   {

   float value;

   float G = sqrt( 1 - cos(a)*cos(a)
                     - cos(b)*cos(b)
                     - cos(c)*cos(c)
                     + 2*cos(a)*cos(b)*cos(c)  );

   float P = 1 / ( sqrt (1 + (2 * cos (a)) ) );
   float Q = 1 / ( sqrt (1 - (1 * cos (a)) ) );

   if      (type == 0)
      value = ( X1 * sin(a) / (A * G) );

   else if (type == 1)
      value = ( + ( X1 / A                                          )
                - ( Y1 / (A * tan(c) )                              )
                + ( Z1 * ( cos(a)*cos(c)-cos(b) )/(A * G * sin (c)) ) );

   else
      value = (  ( + X1 * ((1/P) + (2/Q))
                   + Y1 * ((1/P) - (1/Q))
                   + Z1 * ((1/P) - (1/Q))  )
                   / (3 * A)                  );

   return (value * X_CELL);

   }

//**************************************************************************
//** Calculate X in real coordinates from X in fractional coordinates     **
//**************************************************************************

float    C2C_x( int type,
                float x1, float y1, float z1,
                float A , float B , float C ,
                float a , float b , float c   )
   {

   float G = sqrt( 1 - cos(a)*cos(a)
                     - cos(b)*cos(b)
                     - cos(c)*cos(c)
                     + 2*cos(a)*cos(b)*cos(c)  );

   float P = 1 / ( sqrt (1 + (2 * cos (a)) ) );
   float Q = 1 / ( sqrt (1 - (1 * cos (a)) ) );

   if      (type == 0)
      return ( x1 * A * G / sin(a));

   else if (type == 1)
      return ( + ( x1 * A            )
               + ( y1 * B * cos(c)   )
               + ( z1 * C * cos(b)   )  );

   else
      return (  ( + x1 * ((1*P) + (2*Q))
                  + y1 * ((1*P) - (1*Q))
                  + z1 * ((1*P) - (1*Q))  )
                  * (A / 3)                  );
   }

//**************************************************************************
//** Calculate Y in fractional coordinates from Y in real coordinates     **
//**************************************************************************

int      C2C_Y( int type,
                float X1, float Y1, float Z1,
                float A , float B , float C ,
                float a , float b , float c   )
   {

   float value;

   float G = sqrt( 1 - cos(a)*cos(a)
                     - cos(b)*cos(b)
                     - cos(c)*cos(c)
                     + 2*cos(a)*cos(b)*cos(c)  );

   float P = 1 / ( sqrt (1 + (2 * cos (a)) ) );
   float Q = 1 / ( sqrt (1 - (1 * cos (a)) ) );

   if      (type == 0)
      value = ( + ( X1 * ( cos(a)*cos(b) - cos(c) ) / (B * G * sin(a)) )
                + ( Y1 / (B * sin(a) )                                 ) );

   else if (type == 1)
      value = ( + ( Y1 / (B * sin(c) )                                 )
                + ( Z1 * ( cos(b)*cos(c) - cos(a) ) / (B * G * sin(c)) ) );

   else
      value = (  ( + X1 * ((1/P) - (1/Q))
                   + Y1 * ((1/P) + (2/Q))
                  + Z1 * ((1/P) - (1/Q))  )
                  / (3 * A)                  );

   return (value * Y_CELL);

   }

//**************************************************************************
//** Calculate Y in real coordinates from Y in fractional coordinates     **
//**************************************************************************

float    C2C_y( int type,
                float x1, float y1, float z1,
                float A , float B , float C ,
                float a , float b , float c   )
   {

   float G = sqrt( 1 - cos(a)*cos(a)
                     - cos(b)*cos(b)
                     - cos(c)*cos(c)
                     + 2*cos(a)*cos(b)*cos(c)  );

   float P = 1 / ( sqrt (1 + (2 * cos (a)) ) );
   float Q = 1 / ( sqrt (1 - (1 * cos (a)) ) );

   if      (type == 0)
      return ( + ( x1 * A * ( cos(c) - cos(a)*cos(b) ) / sin(a) )
               + ( y1 * B * sin(a)                              ) );

   else if (type == 1)
      return ( + ( y1 * B * sin(c)                              )
               + ( z1 * C * ( cos(a) - cos(b)*cos(c) ) / sin(c) ) );

   else
      return (  ( + x1 * ((1*P) - (1*Q))
                  + y1 * ((1*P) + (2*Q))
                  + z1 * ((1*P) - (1*Q))  )
                  * (A / 3)                  );
   }

//**************************************************************************
//** Calculate Z in fractional coordinates from Z in real coordinates     **
//**************************************************************************

int      C2C_Z( int type,
                float X1, float Y1, float Z1,
                float A , float B , float C ,
                float a , float b , float c   )
   {

   float value;

   float G = sqrt( 1 - cos(a)*cos(a)
                     - cos(b)*cos(b)
                     - cos(c)*cos(c)
                     + 2*cos(a)*cos(b)*cos(c)  );

   float P = 1 / ( sqrt (1 + (2 * cos (a)) ) );
   float Q = 1 / ( sqrt (1 - (1 * cos (a)) ) );

   if      (type == 0)
      value = ( + ( X1 * ( cos(a)*cos(c)-cos(b) ) / (C * G * sin (a)) )
                - ( Y1 / (C * tan(a) )                                )
                + ( Z1 / C                                            ) );

   else if (type == 1)
      value = ( Z1 * sin(c) / (C * G) );

   else
      value = (  ( + X1 * ((1/P) - (1/Q))
                   + Y1 * ((1/P) - (1/Q))
                   + Z1 * ((1/P) + (2/Q))  )
                   / (3 * A)                  );

   return (value * Z_CELL);

   }

//**************************************************************************
//** Calculate Z in real coordinates from Z in fractional coordinates     **
//**************************************************************************

float    C2C_z( int type,
                float x1, float y1, float z1,
                float A , float B , float C ,
                float a , float b , float c   )
   {

   float G = sqrt( 1 - cos(a)*cos(a)
                     - cos(b)*cos(b)
                     - cos(c)*cos(c)
                     + 2*cos(a)*cos(b)*cos(c)  );

   float P = 1 / ( sqrt (1 + (2 * cos (a)) ) );
   float Q = 1 / ( sqrt (1 - (1 * cos (a)) ) );

   if      (type == 0)
      return ( + ( x1 * A * cos(b)  )
               + ( y1 * B * cos(a)  )
               + ( z1 * C           ) );

   else if (type == 1)
      return ( z1 * C * G / sin(c)  );

   else
      return (  ( + z1 * ((1*P) - (1*Q))
                  + y1 * ((1*P) - (1*Q))
                  + z1 * ((1*P) + (2*Q))  )
                  * (A / 3)                  );
   }

//**************************************************************************
//** INT function:  Integrates electron density for sphere.               **
//**************************************************************************

float Int(int X, int Y, int Z, float r, int map1)
   {

   register int   countz;
   register int   county;
   register int   countx;

   register int   dZ;
   register int   dY;
   register int   dX;

   register int   maxX;
   register int   minX;

   register int   maxY;
   register int   minY;

   register int   maxZ;
   register int   minZ;

   register int   LOC;

   register int   num;

   register double value = 0;

   minX = X - (r/X_GRID) - 1;   if (minX < 1)     minX = 1;
   maxX = X + (r/X_GRID) + 1;   if (maxX > X_LIM) maxX = X_LIM;

   minY = Y - (r/Y_GRID) - 1;   if (minY < 1)     minY = 1;
   maxY = Y + (r/Y_GRID) + 1;   if (maxY > Y_LIM) maxY = Y_LIM;

   minZ = Z - (r/Z_GRID) - 1;   if (minZ < 1)     minZ = 1;
   maxZ = Z + (r/Z_GRID) + 1;   if (maxZ > Z_LIM) maxZ = Z_LIM;

   for (countz = minZ; countz <= maxZ; countz ++)
      for (county = minY; county <= maxY; county ++)
         for (countx = minX; countx <= maxX; countx ++)
            {
            LOC = ((countx - 1)          ) +
                  ((county - 1) * X_LIM  ) +
                  ((countz - 1) * XY_LIM );

            dX = Abs(X-countx);
            dY = Abs(Y-county);
            dZ = Abs(Z-countz);

            if (distance(dX, dY, dZ) > r) continue;

            num ++;

            value = value + MAP[LOC + (map1 * XYZ_LIM)];
            }

   value = value * vox_vol;

   return value;

   }

//**************************************************************************
//** INTEGRATE function:  Integrates density for all atoms in pdb file    **
//**************************************************************************

void  Integrate(int pdb1, int map1)
   {

   register int count1;
   register int LOC;

   for (count1 = 1; count1 <= pdb_len[pdb1]; count1 ++)
      {

      LOC = ((pdb_mem * pdb1) + count1);

      if (!PDB[LOC].Type) continue;

      PDB[LOC].Enum = Int(   PDB[LOC].X,
                             PDB[LOC].Y,
                             PDB[LOC].Z,
                             PDBdat[PDB[LOC].Type].r,
                             map1                      );

      }

   }

//**************************************************************************
//** HELP function:  Displays how to use the program                      **
//**************************************************************************

void  Help()
   {
   cout
<<"\n\n"
<<"************************************************************************\n"
<<"*                                                                      *\n"
<<"* The program is controlled by KEYWORDS, which are described here.     *\n"
<<"* In general, the first 5 letters of a keyword are significant         *\n"
<<"* (all letters in shorter keywords are significant).  In general,      *\n"
<<"* wherever the words IN or OUT determine the area where an operation   *\n"
<<"* is to be carried out, the word TOTAL may also be used to indicate    *\n"
<<"* the operation is to be carried out everywhere, but NO MASK WILL      *\n"
<<"* BE INPUT WHEN THE KEYWORD 'TOTAL' IS USED:                           *\n"
<<"*                                                                      *\n"
<<"*    HELP  => Displays this information to screen.                     *\n"
<<"*    KEYS  => Displays just the keys and command formats to screen.    *\n"
<<"*    LIST  => Lists maps and masks in memory, with original load name. *\n"
<<"*                                                                      *\n"
<<"*    MAPIN X1 'name'                                                   *\n"
<<"*          => Input a map of name 'name' into variable location X1.    *\n"
<<"*             This map will from then on be referenced by its number   *\n"
<<"*             X1.  All information in variable number X1 will be       *\n"
<<"*             overwritten with no warning.  Note that the program      *\n"
<<"*             begins with a call to MAPIN 1, with the name given on    *\n"
<<"*             the command line.  Consequently, the first action taken  *\n"
<<"*             by the RsRf program is to input a map with the name given*\n"
<<"*             at the command line.  ALL OTHER MAPS AND MASKS INPUT TO  *\n"
<<"*             THE PROGRAM MUST HAVE THE SAME NUMBER OF ROWS, COLUMNS,  *\n"
<<"*             AND SECTIONS AS THIS FIRST COMMAND LINE INPUT MAP.       *\n"
<<"*    MASKI Y1 'name'                                                   *\n"
<<"*          => Input a mask of name 'name' into variable location Y1.   *\n"
<<"*             This mask will from then on be referenced by its number  *\n"
<<"*             Y1.                                                      *\n"
<<"*                                                                      *\n"
<<"*    SCALE X1 X2 IN/OUT/TOTAL Y1                                       *\n"
<<"*          => Scale map X1 to map X2 IN or OUT of mask Y1, where X1,   *\n"
<<"*             X2, and Y1 are numbers of previously loaded maps/masks.  *\n"
<<"*             Map X1  will be multiplied by a constant such that the   *\n"
<<"*             sum of density inside/outside of mask Y1 region          *\n"
<<"*             is the same in both maps.  This is the only operation    *\n"
<<"*             which modifies the input maps in a typical application.  *\n"
<<"*          => Example 1:  ?SCALE 1 2 IN 1 (would scale input map       *\n"
<<"*             number 1 to input map number 2 inside of mask 1).        *\n"
<<"*          => Example 2:  ?SCALE 1 2 TOTAL (would scale input map      *\n"
<<"*             number 1 to input map number 2 everywhere).              *\n"
<<"*    RFAC X1 X2 IN/OUT/TOTAL Y1                                        *\n"
<<"*          => Find R factor between map X1 and map X2 INR or OUT of    *\n"
<<"*             mask Y1.                                                 *\n"
<<"*          => Example:  ?RFAC 1 2 OUT 1                                *\n"
<<"*             Finds R factor between map 1 and map 2 outside of        *\n"
<<"*             mask 1.                                                  *\n"
<<"*    RMS  X1 IN/OUT/TOTAL Y1                                           *\n"
<<"*          => Find root mean square variance in map density for        *\n"
<<"*             map X1 IN or OUT of mask Y1.                             *\n"
<<"*          => Example:  ?RMS 1 OUT 1                                   *\n"
<<"*             Finds root mean square variance in map density for       *\n"
<<"*             map 1 outside of mask 1.                                 *\n"
<<"*    SMEAR X1 X2 X3 N                                                  *\n"
<<"*          => Smooth map X1 by convolution with linear density         *\n"
<<"*             sphere and save in location X2.  Memory location X3 is   *\n"
<<"*             used for temporary calculations.                         *\n"
<<"*          => Example:  ?SMEAR 1 3 4 3                                 *\n"
<<"*             Smooths map 1 by spreading out density in one pixel to   *\n"
<<"*             three additional pixels in all directions, and saves in  *\n"
<<"*             memory location 3.                                       *\n"
<<"*                                                                      *\n"
<<"*    ZERO X1 IN/OUT/TOTAL Y1                                           *\n"
<<"*          => Assigns zero to all pixels in map X1 which are IN/OUT of *\n"
<<"*             mask Y1.                                                 *\n"
<<"*          => Example:  ?ZERO 3 OUT 1 (would assign zero value to all  *\n"
<<"*             pixels in map 3 inside of mask 1).                       *\n"
<<"*    CUT X1 IN/OUT/TOTAL Y1 MIN MAX                                    *\n"
<<"*          => Truncates any value in map X1 IN/OUT of mask Y1 that     *\n"
<<"*             is above MAX or bellow MIN.                              *\n"
<<"*          => Example:  ?CUT 3 OUT 1 -0.100 0.600                      *\n"
<<"*             Truncates all electron densities in map 3 outside of     *\n"
<<"*             mask 1 with values bellow -0.100 e/A^3 or above          *\n"
<<"*             0.600 e/A^3.                                             *\n"
<<"*    MAXOF X1 X2 X3                                                    *\n"
<<"*          => Combines maps X2 and X3 into map X1 such that each       *\n"
<<"*             pixel of X1 contains the highest value of X2 and X3      *\n"
<<"*          => Example:  ?MAXOF 3 1 2                                   *\n"
<<"*                                                                      *\n"
<<"*    MAXMS X1 X2 X3                                                    *\n"
<<"*          => Combines masks Y2 and Y3 into map Y1 such that each      *\n"
<<"*             pixel of Y1 is 1 if either X2 or X3 is 1.                *\n"
<<"*          => Example:  ?MAXMS 3 1 2                                   *\n"
<<"*                                                                      *\n"
<<"*    MINMS X1 X2 X3                                                    *\n"
<<"*          => Combines masks Y2 and Y3 into map Y1 such that each      *\n"
<<"*             pixel of Y1 is 0 if either X2 or X3 is 0.                *\n"
<<"*          => Example:  ?MINMS 3 1 2                                   *\n"
<<"*                                                                      *\n"
<<"*    FLIP  Y1 Y2                                                       *\n"
<<"*          => Flips 1 and 0 in mask Y2 and saves in mask Y1.           *\n"
<<"*                                                                      *\n"
<<"*    ADD X1 X2 IN/OUT/TOTAL Y1                                         *\n"
<<"*          => Add map X2 to map X1 IN/OUT of mask Y1.                  *\n"
<<"*          => Example:  ?ADD 3 1 OUT 1                                 *\n"
<<"*             Adds map 1 to map 3 outside of mask 1 (note that map 1   *\n"
<<"*             is not modified, but map 3 has all pixels outside of     *\n"
<<"*             mask 1 incremented by the equivalent values in map 1).   *\n"
<<"*    SUB X1 X2 IN/OUT/TOTAL Y1                                         *\n"
<<"*          => Subtract map X2 from map X1 IN/OUT of mask Y1.           *\n"
<<"*          => Example:  ?SUB 3 1 OUT 1                                 *\n"
<<"*             Subtracts map 1 from map 3 outside of mask 1.            *\n"
<<"*    COMB X1 X2 IN/OUT/TOTAL Y1 float                                  *\n"
<<"*          => Adds X2 times a float to X1 IN/OUT of mask Y1.           *\n"
<<"*          => Example 1: ? COMB 3 1 OUT 1 -1.0                         *\n"
<<"*             Subtracts map 1 from map 3 outside of mask 1.            *\n"
<<"*          => Example 2: ? COMB 3 1 OUT 1 +1.0                         *\n"
<<"*             Adds map 1 to map 3 outside of mask 1                    *\n"
<<"*          => Example 3: ? COMB 3 1 OUT 1 +0.25                        *\n"
<<"*             Adds one quarter of map 1 to map 3 outside of mask 1     *\n"
<<"*                                                                      *\n"
<<"*    AVG X1 IN/OUT/TOTAL Y1                                            *\n"
<<"*          => Finds the average electron density of map X1 IN/OUT of   *\n"
<<"*             mask Y1 and saves that value in the storage variable.    *\n"
<<"*          => Example: AVG 1 IN 1                                      *\n"
<<"*             Stores the average value of map 1 inside mask 1.         *\n"
<<"*    PLUS X1 IN/OUT/TOTAL Y1 VALUE/float                               *\n"
<<"*          => Adds a constant to map X1 IN/OUT of mask Y1.             *\n"
<<"*             The constant is either the value in the storage variable *\n"
<<"*             (this is done when VALUE is entered) or a float given    *\n"
<<"*             by the user.                                             *\n"
<<"*          => Example 1: PLUS 1 IN 1 VALUE                             *\n"
<<"*             Adds the value of the storage variable to map 1          *\n"
<<"*             inside mask 1.                                           *\n"
<<"*          => Example 2: PLUS 1 IN 1 -0.750                            *\n"
<<"*             Subtracts 0.750 from every pixel in map 1 inside mask 1  *\n"
<<"*    MULT X1 IN/OUT/TOTAL Y1 float                                     *\n"
<<"*          => Multiplies map X1 by a float IN/OUT of mask Y1.          *\n"
<<"*          => Example:  MULT 1 IN 1 0.500                              *\n"
<<"*             Divides all density in map 1 and within mask 1 by 2.0    *\n"
<<"*    NEG      Transforms the storage variable into its negative.       *\n"
<<"*                                                                      *\n"
<<"*    WRITE X1 'name'                                                   *\n"
<<"*          => Write map number X1 to file named 'name'.                *\n"
<<"*          => Example:  ?WRITE 3 out.map                               *\n"
<<"*    MASKO Y1 'name'                                                   *\n"
<<"*          => Write mask number Y1 to file named 'name'.               *\n"
<<"*                                                                      *\n"
<<"*    GRAY 'name' n X1 X1Start X1Step ... Xn XnStart XnStep             *\n"
<<"*         x1 x2 y1 y2 z1 z2                                            *\n"
<<"*          => Write grayscale output.  n maps are output, with density *\n"
<<"*             and density step size specified for each map.  Output    *\n"
<<"*             grayscale file will have name 'name'.  Output will       *\n"
<<"*             consist of a block defined by x1 to x2, y1 to y2, and    *\n"
<<"*             z1 to z2.   Each of these may be entered as -1 to mean   *\n"
<<"*             the map min/max.                                         *\n"
<<"*          => Example:  ?GRAY out.gray 3                               *\n"
<<"*                               1 -0.50  0.0150                        *\n"
<<"*                               2 -0.50  0.0150                        *\n"
<<"*                               3 -0.25  0.0025                        *\n"
<<"*                              -1 -1                                   *\n"
<<"*                              -1 -1                                   *\n"
<<"*                               1  1                                   *\n"
<<"*             Outputs three maps to a grayscale file, maps 1, 2, and   *\n"
<<"*             3.  Map 1 begins at density -0.50 e/A^3 and each gray    *\n"
<<"*             step is equal to 0.0150 density units, and so forth.     *\n"
<<"*             A single section is output, of the full x and y map.     *\n"
<<"*                                                                      *\n"
<<"*    END/QUIT/STOP                                                     *\n"
<<"*          => Quits program.                                           *\n"
<<"*                                                                      *\n"
<<"************************************************************************\n"
<<"************************************************************************\n"
<<"\n\n";

   return;
   }

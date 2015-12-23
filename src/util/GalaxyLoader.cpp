#include "GalaxyLoader.h"

float posFactor = 0.05f;
float massFactor = 10.0f;
float velFactor = 1.0f;

extern GLuint Texture;
extern GLuint TextureID;

Star** getStars(char* filepath, int maxSize, int glx_name, int glx_type, int* size)
{
    string line;
    ifstream file;
    int i;

    Star** stars = new Star*[maxSize];
    
    file.open(filepath);
    if(file.is_open()){
        i = 0;
        while(!file.eof()){
            if(i >= maxSize){
                fprintf(stderr, "Read: %d, Max: %d, Stopping\n", i, maxSize);
                break;
            }
            float mass;
            float x, y, z;
            float vx, vy, vz;

            getline(file, line);
            
            //fprintf(stderr, "%d-th Line: %s \n", i, line.c_str());
            
            stringstream iss(line);
            string word;
            int w = 0;
            while (iss >> word) {
                if (w == 0)
                {
                    mass = stof(word);
                    mass *= massFactor;
                }
                else if (w == 1)
                {
                    x = stof(word);
                    x *= posFactor;
                }
                else if (w == 2)
                {
                    y = stof(word);
                    y *= posFactor;
                }
                else if (w == 3)
                {
                    z = stof(word);
                    z *= posFactor;
                } 
                else if (w == 4)
                {
                    vx = stof(word);
                    vx *= velFactor;
                }
                else if (w == 5)
                {
                    vy = stof(word);
                    vy *= velFactor;
                }
    
                else if (w == 6)
                {
                    vz = stof(word);
                    vz *= velFactor;
                }

                w++;
            }

            Star* star = new Star(mass, x, y, z,
                vx, vy, vz, Texture, TextureID);

            star->setGalaxyType(glx_name);
            star->setStarType(glx_type);
            stars[i++] = star;
        }   
    }
    *size = i;
    return stars;
}

Galaxy** glxParseGalaxies(const char* dir)
{
    Galaxy** galaxies;
    char filepath[FILENAME_SIZE];
    
    Star** mw_disk;
    Star** mw_bulge;
    Star** mw_halo;
    
    Star** and_disk;
    Star** and_bulge;
    Star** and_halo;
    
    int mw_disk_size;
    int mw_bulge_size;
    int mw_halo_size;
    
    int and_disk_size;
    int and_bulge_size;
    int and_halo_size;
    
    fprintf(stderr, "\n\n**** Loading Galaxy samples ****\n\n");
    fprintf(stderr, "Sample dir: %s\n\n", dir);
    
    galaxies = new Galaxy*[GALAXY_COUNT];
    
    /********* MILKY WAY *********/
    
    /// READ MW DISK
    if(snprintf(filepath, FILENAME_SIZE, "%s/%s%s", 
            dir, MW_FILENAME, DISK_EXT) < 0) ERR("snprintf");

    fprintf(stderr, "Opening file: %s\n", filepath);
    
    mw_disk = getStars(filepath, MAX_MW_DISK_COUNT, GLX_MW, GLX_DISK, &mw_disk_size);
    
    fprintf(stderr, "Read MW_DISK: %d stars\n\n", mw_disk_size);
    
    /// READ MW BULGE
    if(snprintf(filepath, FILENAME_SIZE, "%s/%s%s", 
            dir,MW_FILENAME, BULGE_EXT) < 0) ERR("snprintf");

    fprintf(stderr, "Opening file: %s\n", filepath);
    
    mw_bulge = getStars(filepath, MAX_MW_BULGE_COUNT, GLX_MW, GLX_BULGE, &mw_bulge_size);
    
    fprintf(stderr, "Read MW_BULGE: %d stars\n\n", mw_bulge_size);
    
    
    /// READ MW BULGE
    if(snprintf(filepath, FILENAME_SIZE, "%s/%s%s", 
            dir, MW_FILENAME, HALO_EXT) < 0) ERR("snprintf");

    fprintf(stderr, "Opening file: %s\n", filepath);
    
    mw_halo = getStars(filepath, MAX_MW_HALO_COUNT, GLX_MW, GLX_HALO, &mw_halo_size);
    
    fprintf(stderr, "Read MW_HALO: %d stars\n\n", mw_halo_size);
    
    galaxies[0] = new Galaxy(mw_disk, mw_bulge, mw_halo, 
                            mw_disk_size, mw_bulge_size, mw_halo_size);
    
    fprintf(stderr, "Galaxy1 count: %d stars\n\n", mw_halo_size + mw_bulge_size + mw_disk_size);
    
    /********* AND *********/
    
    /// READ AND DISK
    if(snprintf(filepath, FILENAME_SIZE, "%s/%s%s", 
            dir, AND_FILENAME, DISK_EXT) < 0) ERR("snprintf");

    fprintf(stderr, "Opening file: %s\n", filepath);
    
    and_disk = getStars(filepath, MAX_AND_DISK_COUNT, GLX_AND, GLX_DISK, &and_disk_size);
    
    fprintf(stderr, "Read AND_DISK: %d stars\n\n", and_disk_size);
    
    /// READ AND BULGE
    if(snprintf(filepath, FILENAME_SIZE, "%s/%s%s", 
            dir, AND_FILENAME, BULGE_EXT) < 0) ERR("snprintf");

    fprintf(stderr, "Opening file: %s\n", filepath);
    
    and_bulge = getStars(filepath, MAX_AND_BULGE_COUNT, GLX_AND, GLX_BULGE, &and_bulge_size);
    
    fprintf(stderr, "Read AND_BULGE: %d stars\n\n", and_bulge_size);
    
    
    /// READ AND BULGE
    if(snprintf(filepath, FILENAME_SIZE, "%s/%s%s", 
            dir, AND_FILENAME, HALO_EXT) < 0) ERR("snprintf");

    fprintf(stderr, "Opening file: %s\n", filepath);
    
    and_halo = getStars(filepath, MAX_AND_HALO_COUNT, GLX_AND, GLX_HALO, &and_halo_size);
    
    fprintf(stderr, "Read AND_HALO: %d stars\n\n", and_halo_size);
    
    galaxies[1] = new Galaxy(and_disk, and_bulge, and_halo, 
                            and_disk_size, and_bulge_size, and_halo_size);
    
    fprintf(stderr, "Galaxy2 count: %d stars\n\n", and_halo_size + and_bulge_size + and_disk_size);

    fprintf(stderr, "Galaxy1 count: %d\n", galaxies[0]->TotalSize());
    fprintf(stderr, "Galaxy2 count: %d\n", galaxies[1]->TotalSize());

    printSampleStars(galaxies);
    
    return galaxies;
}

void printSampleStars(Galaxy** galaxies)
{
    cout << "\n" << endl;
    
    if((galaxies[0]->diskSize) > 0)
        cout << *(galaxies[0]->disk[0]) << endl;
    if((galaxies[0]->bulgeSize) > 0)
        cout << *(galaxies[0]->bulge[0]) << endl;
    if((galaxies[0]->haloSize) > 0)
        cout << *(galaxies[0]->halo[0]) << endl;
    
    
    if((galaxies[1]->diskSize) > 0)
        cout << *(galaxies[1]->disk[0]) << endl;
    if((galaxies[1]->bulgeSize) > 0)
        cout << *(galaxies[1]->bulge[0]) << endl;
    if((galaxies[1]->haloSize) > 0)
        cout << *(galaxies[1]->halo[0]) << endl;
    
    cout << "\n" << endl;
}

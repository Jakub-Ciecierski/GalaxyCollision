#include "Star.h"

Star::Star()
{
    this->mass = 0;

    this->x = 0;
    this->y = 0;
    this->z = 0;

    this->vx = 0;
    this->vy = 0;
    this->vz = 0;

    //sphere = new Sphere(this->x, this->y, this->z, width);
}

Star::Star(float mass, float x, float y, float z,
    float vx, float vy, float vz)
{
    this->mass = mass;

    this->x = x;
    this->y = y;
    this->z = z;
    
    this->vx = vx;
    this->vy = vy;
    this->vz = vz;
    
    sphere = new Sphere(this->x, this->y, this->z);
}

Star::Star(float mass, float x, float y, float z,
    float vx, float vy, float vz, GLuint Texture, GLuint TextureID)
{
    this->mass = mass;

    this->x = x;
    this->y = y;
    this->z = z;

    this->vx = vx;
    this->vy = vy;
    this->vz = vz;



    //sphere = new Sphere(this->x, this->y, this->z, Texture, TextureID);
    point = new Point(this->x, this->y, this->z, Texture, TextureID);
}

void Star::setGalaxyType(int galaxyType)
{
    this->galaxyType = galaxyType;
}

void Star::setStarType(int starType)
{
    this->starType = starType;
}

void Star::Update(float ax, float ay, float az)
{
    // TODO add deltaTime multiplyer

    vx += ax * speed;
    vy += ay * speed;
    vz += az * speed;

    this->point->X += vx * speed;
    this->point->Y += vy * speed;
    this->point->Z += vz * speed;
    
    float r, g, b;
    
    if(vx > vy && vx > vz)
    {
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
    }
    else if(vy > vx && vy > vz)
    {
        r = 0.0f;
        g = 1.0f;
        b = 0.0f;
    }
    else if(vz > vy && vz > vx)
    {
        r = 0.0f;
        g = 0.0f;
        b = 1.0f;
    }
    else
    {
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
    }
        
    this->point->UpdateColor(r, g, b);
}

void Star::Update(float ax, float ay, float az, float maxVelocity)
{
    // TODO add deltaTime multiplyer

    vx += ax * speed;
    vy += ay * speed;
    vz += az * speed;

    this->point->X += vx * speed;
    this->point->Y += vy * speed;
    this->point->Z += vz * speed;
    
    float myVelocity = this->GetVelocityLength();
    
    float diff = (myVelocity / maxVelocity);
    //printf("%f\n",diff);
    
    if(this->galaxyType == GLX_MW){
        this->point->R = 1.0f - diff;
        this->point->G = 1.0f;
        this->point->B = diff;
    }
    else
    {
        this->point->R = diff;
        this->point->G = 1.0f;
        this->point->B = 1.0f - diff;
    }
    //this->point->UpdateColor(diff, diff, diff);
    //this->point->UpdateColor(, 1.0f, 1.0f);
   
}

Sphere* Star::GetSphere()
{
    return this->sphere;
}     

Point* Star::GetPoint()
{
    return this->point;
}     

float Star::GetVelocityLength()
{
    float length = 0;
    
    length = (vx*vx + vy*vy + vz*vz);
    length = sqrt(length);
    
    return length;
}

 ostream& operator<< (ostream& os, Star& star)
 {
    std::string galaxy_type;
    std::string star_type;

    if(star.galaxyType == GLX_MW)
        galaxy_type = "Milky Way";
    else if(star.galaxyType == GLX_AND)
        galaxy_type = "Andromeda";
    else
        galaxy_type = "Unknown";
    if(star.starType == GLX_DISK)
        star_type = "Disk";
    else if(star.starType == GLX_BULGE)
        star_type = "Bulge";
    else if(star.starType == GLX_HALO)
        star_type = "Halo";
    else
        star_type = "Unknown";            
                 
    os << "Galaxy: " << galaxy_type
            << ", "
            << "Type: " << star_type
            << "\n"
            << "Mass: " << star.mass
            << "\n"
            << "Position (x,y,z): " 
                << "(" << star.x << ", " 
                << star.y << ", " 
                << star.z << ")"
            << endl;
    return os;
 }
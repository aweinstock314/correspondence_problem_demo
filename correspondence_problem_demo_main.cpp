#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <MersenneTwister.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <string.h>
#include "texture.c"

#define WIDTH 640
#define HEIGHT 480
#define PI 3.1415926535

//if GENERATE_TIKZ_OUTPUT is enabled, LaTeX+TIKZ commands to draw the uvmaps will be displayed to stdout, (intended to be used with output redirection)
#define GENERATE_TIKZ_OUTPUT 0

#define DEFINEOPERATOR2(symbol) \
tuple2 operator symbol(const tuple2& other) \
{ \
  tuple2 temp; \
  temp.x = this->x symbol other.x; \
  temp.y = this->y symbol other.y; \
  return temp; \
}

template<class T> class tuple2
{
  public:
  tuple2()
  {
    
  }
  tuple2(T a,T b)
  {
    x = a;
    y = b;
  }
  DEFINEOPERATOR2(+)
  DEFINEOPERATOR2(-)
  DEFINEOPERATOR2(*)
  DEFINEOPERATOR2(/)
  tuple2& operator[](int index)
  {
    switch(index)
    {
      case 0: return &x; break;
      case 1: return &y; break;
      default: return NULL; break;
    }
  }
  T x;
  T y;
};

template<class T> tuple2<T> make_tuple2(T a,T b)
{
  tuple2<T> tmp;
  tmp.x = a;
  tmp.y = b;
  return tmp;
}

#define DEFINEOPERATOR3(symbol) \
tuple3 operator symbol(const tuple3& other) \
{ \
  tuple3 temp; \
  temp.x = this->x symbol other.x; \
  temp.y = this->y symbol other.y; \
  temp.z = this->z symbol other.z; \
  return temp; \
}

template<class T> class tuple3
{
  public:
  tuple3()
  {
    
  }
  tuple3(T a,T b,T c)
  {
    x = a;
    y = b;
    z = c;
  }
  DEFINEOPERATOR3(+)
  DEFINEOPERATOR3(-)
  DEFINEOPERATOR3(*)
  DEFINEOPERATOR3(/)
  tuple3& operator[](int index)
  {
    switch(index)
    {
      case 0: return &x; break;
      case 1: return &y; break;
      case 2: return &z; break;
      default: return NULL; break;
    }
  }
  T x;
  T y;
  T z;
};

template<class T> tuple3<T> make_tuple3(T a,T b,T c)
{
  tuple3<T> tmp;
  tmp.x = a;
  tmp.y = b;
  tmp.z = c;
  return tmp;
}

#define DEFINEOPERATOR4(symbol) \
tuple4 operator symbol(const tuple4& other) \
{ \
  tuple4 temp; \
  temp.w = this->w symbol other.w; \
  temp.x = this->x symbol other.x; \
  temp.y = this->y symbol other.y; \
  temp.z = this->z symbol other.z; \
  return temp; \
}

template<class T> class tuple4
{
  public:
  DEFINEOPERATOR4(+)
  DEFINEOPERATOR4(-)
  DEFINEOPERATOR4(*)
  DEFINEOPERATOR4(/)
  tuple4& operator[](int index)
  {
    switch(index)
    {
      case 0: return &w; break;
      case 1: return &x; break;
      case 2: return &y; break;
      case 3: return &z; break;
      default: return NULL; break;
    }
  }
  T w;
  T x;
  T y;
  T z;
};

template<class T> tuple4<T> make_tuple4(T a,T b,T c,T d)
{
  tuple4<T> tmp;
  tmp.w = a;
  tmp.x = b;
  tmp.y = c;
  tmp.z = d;
  return tmp;
}

#define absolute(x) (((x)<0)?(-(x)):(x))

float rand_float(float min,float max)
{
   MTRand mtrandom;
   return float(mtrandom.rand(max-min)+min);
}

struct vertex_type
{
  tuple3<float> pos;
  tuple2<float> texcoords;
  tuple4<float> color;
};

struct triangle_type
{
  vertex_type verts[3];
};

tuple4<float> random_color()
{
  return make_tuple4<float>(rand_float(0,1),rand_float(0,1),rand_float(0,1),/*rand_float(.5,1)*/.75);
}

triangle_type triangle_from_points(tuple3<float> a, tuple3<float> b, tuple3<float> c, tuple4<float> color)
{
  triangle_type retval;
  for(int c1=0;c1<3;c1++)
  {
    retval.verts[c1].color = color;
  }
  retval.verts[0].pos = a;
  retval.verts[1].pos = b;
  retval.verts[2].pos = c;
  return retval;
}

triangle_type triangle_from_points(tuple3<float> a, tuple3<float> b, tuple3<float> c, tuple3<tuple2<float> > uv, tuple4<float> color)
{
  triangle_type retval;
  for(int c1=0;c1<3;c1++)
  {
    retval.verts[c1].color = color;
  }
  retval.verts[0].pos = a;
  retval.verts[1].pos = b;
  retval.verts[2].pos = c;
  retval.verts[0].texcoords = uv.x;
  retval.verts[1].texcoords = uv.y;
  retval.verts[2].texcoords = uv.z;
  //printf("\\draw (%f,-%f) -- (%f,-%f);\n",uv.x.x,uv.x.y,uv.y.x,uv.y.y);
  //printf("\\draw (%f,-%f) -- (%f,-%f);\n",uv.y.x,uv.y.y,uv.z.x,uv.z.y);
  //printf("\\draw (%f,-%f) -- (%f,-%f);\n",uv.z.x,uv.z.y,uv.x.x,uv.x.y);
  return retval;
}

std::vector<triangle_type> triangles_from_rectangle(tuple3<float> a, tuple3<float> b, tuple3<float> c, tuple3<float> d, tuple4<float> color)
{
  std::vector<triangle_type> retval;
  retval.push_back(triangle_from_points(a,b,c,color));
  retval.push_back(triangle_from_points(b,c,d,color));
  return retval;
}

std::vector<triangle_type> triangles_from_rectangle(tuple3<float> a, tuple3<float> b, tuple3<float> c, tuple3<float> d, float u1, float v1, float u2, float v2, tuple4<float> color)
{
  std::vector<triangle_type> retval;
  retval.push_back(triangle_from_points(a,b,c,make_tuple3(make_tuple2(u1,v1),make_tuple2(u2,v1),make_tuple2(u1,v2)),color));
  retval.push_back(triangle_from_points(b,c,d,make_tuple3(make_tuple2(u2,v1),make_tuple2(u1,v2),make_tuple2(u2,v2)),color));
  return retval;
}

#define rotate_in_plane(plane1,plane2,the_point,rot_amount)\
float plane1 ## plane2 ## _magnitude = sqrt(the_point.plane1 * the_point.plane1 + the_point.plane2 * the_point.plane2);\
float plane1 ## plane2 ## _angle = atan2(the_point.plane2,the_point.plane1);\
tuple3<float> plane1 ## plane2 ## _point = the_point;\
plane1 ## plane2 ## _point.plane1 = plane1 ## plane2 ## _magnitude * cos(plane1 ## plane2 ## _angle + rot_amount);\
plane1 ## plane2 ## _point.plane2 = plane1 ## plane2 ## _magnitude * sin(plane1 ## plane2 ## _angle + rot_amount);

tuple3<float> rotate_point(const tuple3<float>& point, const tuple3<float>& rotation)
{
  tuple3<float> retval;
  rotate_in_plane(x,y,point,rotation.y)
  rotate_in_plane(x,z,xy_point,rotation.x)
  rotate_in_plane(y,z,xz_point,rotation.z)
  retval = yz_point;
  return retval;
}

//obsolete version left in to illustrate the concept behind the possibly more obscure generalized version above
/*tuple3<float> rotate_point(const tuple3<float>& point, float theta_rot, float phi_rot)
{
  float magnitude = sqrt(point.x*point.x + point.y*point.y + point.z*point.z);
  float xz_magnitude = sqrt(point.x*point.x + point.z*point.z);
  float theta = atan2(point.z,point.x); //radians in a circle aligned with xz plane
  float phi = atan2(point.y,xz_magnitude); //radians above the xz plane
  tuple3<float> retval;
  retval.x = xz_magnitude*cos(theta+theta_rot);
  retval.z = xz_magnitude*sin(theta+theta_rot);
  retval.y = magnitude*sin(phi+phi_rot);
  //printf("in (%f,%f,%f) , out (%f,%f,%f)\n",point.x,point.y,point.z,retval.x,retval.y,retval.z);
  return retval;
}*/

class texture_image
{
  public:
  texture_image()
  {
    glGenTextures(1,&texture_id);
    data = NULL;
    change_size(128,128);
  }
  texture_image(const texture_image& other)
  {
    glGenTextures(1,&texture_id);
    data = NULL;
    change_size(other.texture_width,other.texture_height);
    memcpy(data,other.data,texture_width*texture_height*4);
  }
  ~texture_image()
  {
    glDeleteTextures(1,&texture_id);
    free(data);
  }
  void change_size(int new_width, int new_height)
  {
    if(data)free(data);
    data = (unsigned char*)malloc(sizeof(unsigned char)*new_width*new_height*4);
    texture_width = new_width;
    texture_height = new_height;
  }
  void putpixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
  {
    
    data[4*(y*texture_width+x)] = r;
    data[4*(y*texture_width+x)+1] = g;
    data[4*(y*texture_width+x)+2] = b;
    data[4*(y*texture_width+x)+3] = a;
  }
  tuple4<unsigned char> getpixel(int x, int y, unsigned char* raw_data = NULL)
  {
    if(raw_data == NULL)raw_data = this->data;
    return make_tuple4<unsigned char>(raw_data[4*(y*texture_width+x)],raw_data[4*(y*texture_width+x)+1],raw_data[4*(y*texture_width+x)+2],raw_data[4*(y*texture_width+x)+3]);
  }
  void enforce_bounds(int& x, int& y)
  {
    if(x < 0)x = 0;
    if(y < 0)y = 0;
    if(x >= texture_width)x = texture_width-1;
    if(y >= texture_height)y = texture_height-1;
  }
  void drawline(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
  {
    //Bresenham's line drawing algorithm, translated from the wikipedia pseudocode
    int dx = absolute(x2-x1);
    int dy = absolute(y2-y1);
    int sx = (x1<x2)?1:-1;
    int sy = (y1<y2)?1:-1;
    int err = dx-dy;
    
    int x = x1;
    int y = y1;
    for(;;)
    {
      //x and y are clamped to valid ranges with the mod (%) operator to facilitate drawing specific slants of lines:
      //drawline(0,0,2*width,height,...) can be used instead of drawline(0,0,width,height/2,...)
      putpixel(x%texture_width,y%texture_height,r,g,b,a);
      //there were some glitches where the algorithm missed the destination by 1 pixel for some lines (reason unknown)
      //and continued to the edge, so the break condition next line differs from the wikipedia version
      if((absolute(x-x2)<2) && (absolute(y-y2)<2))break;
      if((err*2) > -dy)
      {
        err -= dy;
        x += sx;
      }
      if((err*2) < dx)
      {
        err += dx;
        y += sy;
      }
    }
    //following code fragment was taken from debugging code in main, first line works without my
    //modification, 2nd line misses by 1px and would corrupt memory if not for the mod-clamping
    /*texture_image tex;
    tex.drawline(32,32,54,54,64,64,64,255);
    tex.drawline(54,54,44,61,64,64,64,255);*/
  }
  void apply_texture()
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_id);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //GL_LINEAR or GL_NEAREST
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		//glTexParameterf(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_REPLACE);
		//glTexParameterf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D,0,4,texture_width,texture_height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
  }
  void save_texture(const char* fname)
  {
    FILE* f = fopen(fname,"wb");
    if(f)
    {
      for(int c1=0;c1<texture_width*texture_height*4;c1+=4)
      {
        fprintf(f,"%c%c%c%c",data[c1],data[c1+1],data[c1+2],data[c1+3]);
      }
      fclose(f);
    }
  }
  //the GNU Image Manipulation Program has the option to save images as raw data in a c source file, the following
  //function uses those as textures, with the addition of "GIMP_IMAGE" to the struct type name in the generated file
  void texture_from_gimp(const GIMP_IMAGE& image)
  {
    change_size(image.width,image.height);
    memcpy(data,image.pixel_data,image.width*image.height*4);
  }
  unsigned int texture_id;
  unsigned char* data;
  int texture_width;
  int texture_height;
};

class object3d
{
  public:
  object3d()
  {
    position = make_tuple3<float>(0,0,0);
    rotation = make_tuple3<float>(0,0,0);
    use_uvmap = false;
    uvmap = NULL;
    visible = true;
  }
  ~object3d()
  {
    if(uvmap != NULL)delete uvmap;
  }
  void initialize_uvmap()
  {
    if(uvmap != NULL)delete uvmap;
    uvmap = new texture_image;
  }
  void draw_uvmap_outline()
  {
    if(uvmap == NULL)
    {
      initialize_uvmap();
    }
    MTRand mtrandom;
    
    for(int y=0;y<uvmap->texture_height;y++)
    {
      for(int x=0;x<uvmap->texture_width;x++)
      {
        int lum = mtrandom.randInt(32)+32;
        uvmap->putpixel(x,y,lum,lum,lum,255);
      }
    }
    if(GENERATE_TIKZ_OUTPUT)printf("\\begin{tikzpicture}\n");
    for(int c1=0;c1<triangles.size();c1++)
    {
      for(int c2=0;c2<3;c2++)
      {
        int x1 = triangles[c1].verts[c2].texcoords.x*uvmap->texture_width;
        int y1 = triangles[c1].verts[c2].texcoords.y*uvmap->texture_height;
        int x2 = triangles[c1].verts[(c2+1)%3].texcoords.x*uvmap->texture_width;
        int y2 = triangles[c1].verts[(c2+1)%3].texcoords.y*uvmap->texture_height;
        if(GENERATE_TIKZ_OUTPUT)printf("\\draw (%d,-%d) -- (%d,-%d);\n",x1,y1,x2,y2);
        uvmap->drawline(x1,y1,x2,y2,mtrandom.randInt(127)+128,mtrandom.randInt(127)+128,mtrandom.randInt(127)+128,255);
      }
    }
    if(GENERATE_TIKZ_OUTPUT)printf("\\end{tikzpicture}\n");
  }
  void draw_uvmap_barberpole()
  {
    if(uvmap == NULL)
    {
      initialize_uvmap();
    }
    MTRand mtrandom;
    //draw the end caps with lines to provide fixed points to view rotation
    draw_uvmap_outline();
    //overwrite the sides
    for(int y=uvmap->texture_height/2;y<uvmap->texture_height;y++)
    {
      for(int x=0;x<uvmap->texture_width;x++)
      {
        uvmap->putpixel(x,y,0,0,0,255);
      }
    }
    for(int c1=0;c1<uvmap->texture_width/4;c1++)
    {
      uvmap->drawline(c1,127,c1+uvmap->texture_width/2,64,0,0,255,255);
      uvmap->drawline(c1+uvmap->texture_width/2,127,c1+2*uvmap->texture_width/2,64,255,0,0,255);
    }
    for(int c1=0;c1<1;c1++)
    {
      uvmap->drawline(0,127-c1,uvmap->texture_width,127-c1,255,255,255,255);
      uvmap->drawline(0,64+c1,uvmap->texture_width,64+c1,255,255,255,255);
    }
  }
  bool visible;
  bool use_uvmap;
  texture_image* uvmap;
  tuple3<float> position;
  tuple3<float> rotation;
  std::vector<triangle_type> triangles;
};

object3d* generate_ngon_prism(unsigned int num_sides, float radius, float length)
{
  object3d* obj = new object3d;
  for(unsigned int c1=0;c1<num_sides;c1++)
  {
    float x1 = radius*cos(2*PI*c1/num_sides);
    float z1 = radius*sin(2*PI*c1/num_sides);
    float x2 = radius*cos(2*PI*(c1+1)/num_sides);
    float z2 = radius*sin(2*PI*(c1+1)/num_sides);
    std::vector<triangle_type> tmp = triangles_from_rectangle(make_tuple3(x1,0.0f,z1),make_tuple3(x2,0.0f,z2),make_tuple3(x1,length,z1),make_tuple3(x2,length,z2),random_color());
    for(int c2=0;c2<tmp.size();c2++)
    {
      obj->triangles.push_back(tmp[c2]);
    }
    obj->triangles.push_back(triangle_from_points(make_tuple3<float>(0,0,0),make_tuple3(x1,0.0f,z1),make_tuple3(x2,0.0f,z2),random_color()));
    obj->triangles.push_back(triangle_from_points(make_tuple3<float>(0,length,0),make_tuple3(x1,length,z1),make_tuple3(x2,length,z2),random_color()));
  }
  return obj;
}

object3d* generate_ngon_prism_uv(unsigned int num_sides, float radius, float length)
{
  object3d* obj = new object3d;
  obj->use_uvmap = true;
  for(unsigned int c1=0;c1<num_sides;c1++)
  {
    float x1 = radius*cos(2*PI*c1/num_sides);
    float z1 = radius*sin(2*PI*c1/num_sides);
    float x2 = radius*cos(2*PI*(c1+1)/num_sides);
    float z2 = radius*sin(2*PI*(c1+1)/num_sides);
    float u1 = float(c1)/num_sides;
    float v1 = 1;
    float u2 = float(c1+1)/num_sides;
    float v2 = .5;
    std::vector<triangle_type> tmp = triangles_from_rectangle(make_tuple3(x1,0.0f,z1),make_tuple3(x2,0.0f,z2),make_tuple3(x1,length,z1),make_tuple3(x2,length,z2),u1,v1,u2,v2,random_color());
    for(int c2=0;c2<tmp.size();c2++)
    {
      obj->triangles.push_back(tmp[c2]);
    }
    u1 = .25;
    v1 = .25;
    u2 = u1+.25*x1/radius;
    v2 = v1+.25*z1/radius;
    float u3 = u1+.25*x2/radius;
    float v3 = v1+.25*z2/radius;
    obj->triangles.push_back(triangle_from_points(make_tuple3<float>(0,0,0),make_tuple3(x1,0.0f,z1),make_tuple3(x2,0.0f,z2),make_tuple3(make_tuple2(u1,v1),make_tuple2(u2,v2),make_tuple2(u3,v3)),random_color()));
    u1 = .75;
    v1 = .25;
    u2 = u1+.25*x1/radius;
    v2 = v1+.25*z1/radius;
    u3 = u1+.25*x2/radius;
    v3 = v1+.25*z2/radius;
    obj->triangles.push_back(triangle_from_points(make_tuple3<float>(0,length,0),make_tuple3(x1,length,z1),make_tuple3(x2,length,z2),make_tuple3(make_tuple2(u1,v1),make_tuple2(u2,v2),make_tuple2(u3,v3)),random_color()));
  }
  obj->initialize_uvmap();
  return obj;
}

object3d* generate_ngon_prism(unsigned int num_sides, float radius, tuple3<float> endpoint, tuple4<float> color)
{
  object3d* obj = new object3d;
  bool use_rand_color = (color.z == 0);
  float magnitude = sqrt(endpoint.x*endpoint.x + endpoint.y*endpoint.y + endpoint.z*endpoint.z);
  tuple3<float> rotation;
  rotation.x = atan2(endpoint.z,endpoint.x);
  rotation.y = -acos(endpoint.y/magnitude);
  rotation.z = 0;
  for(unsigned int c1=0;c1<num_sides;c1++)
  {
    float x1 = radius*cos(2*PI*c1/num_sides);
    float z1 = radius*sin(2*PI*c1/num_sides);
    float x2 = radius*cos(2*PI*(c1+1)/num_sides);
    float z2 = radius*sin(2*PI*(c1+1)/num_sides);
    if(use_rand_color)color = random_color();
    std::vector<triangle_type> tmp = triangles_from_rectangle(rotate_point(make_tuple3(x1,0.0f,z1),rotation),rotate_point(make_tuple3(x2,0.0f,z2),rotation),rotate_point(make_tuple3(x1,magnitude,z1),rotation),rotate_point(make_tuple3(x2,magnitude,z2),rotation),color);
    for(int c2=0;c2<tmp.size();c2++)
    {
      obj->triangles.push_back(tmp[c2]);
    }
    if(use_rand_color)color = random_color();
    obj->triangles.push_back(triangle_from_points(rotate_point(make_tuple3<float>(0,0,0),rotation),rotate_point(make_tuple3(x1,0.0f,z1),rotation),rotate_point(make_tuple3(x2,0.0f,z2),rotation),color));
    if(use_rand_color)color = random_color();
    obj->triangles.push_back(triangle_from_points(rotate_point(make_tuple3<float>(0,magnitude,0),rotation),rotate_point(make_tuple3(x1,magnitude,z1),rotation),rotate_point(make_tuple3(x2,magnitude,z2),rotation),color));
  }
  return obj;
}

object3d* generate_ngon_tube(unsigned int num_sides, float radius, tuple3<float> endpoint, tuple4<float> color)
{
  object3d* obj = new object3d;
  bool use_rand_color = (color.z == 0);
  float magnitude = sqrt(endpoint.x*endpoint.x + endpoint.y*endpoint.y + endpoint.z*endpoint.z);
  tuple3<float> rotation;
  rotation.x = atan2(endpoint.z,endpoint.x);
  rotation.y = -acos(endpoint.y/magnitude);
  rotation.z = 0;
  for(unsigned int c1=0;c1<num_sides;c1++)
  {
    float x1 = radius*cos(2*PI*c1/num_sides);
    float z1 = radius*sin(2*PI*c1/num_sides);
    float x2 = radius*cos(2*PI*(c1+1)/num_sides);
    float z2 = radius*sin(2*PI*(c1+1)/num_sides);
    if(use_rand_color)color = random_color();
    std::vector<triangle_type> tmp = triangles_from_rectangle(rotate_point(make_tuple3(x1,0.0f,z1),rotation),rotate_point(make_tuple3(x2,0.0f,z2),rotation),rotate_point(make_tuple3(x1,magnitude,z1),rotation),rotate_point(make_tuple3(x2,magnitude,z2),rotation),color);
    for(int c2=0;c2<tmp.size();c2++)
    {
      obj->triangles.push_back(tmp[c2]);
    }
  }
  return obj;
}

object3d* generate_ngon_prism_uv(unsigned int num_sides, float radius, tuple3<float> endpoint)
{
  object3d* obj = new object3d;
  obj->use_uvmap = true;
  float magnitude = sqrt(endpoint.x*endpoint.x + endpoint.y*endpoint.y + endpoint.z*endpoint.z);
  tuple3<float> rotation;
  rotation.x = atan2(endpoint.z,endpoint.x);
  rotation.y = -acos(endpoint.y/magnitude);
  rotation.z = 0;
  for(unsigned int c1=0;c1<num_sides;c1++)
  {
    float x1 = radius*cos(2*PI*c1/num_sides);
    float z1 = radius*sin(2*PI*c1/num_sides);
    float x2 = radius*cos(2*PI*(c1+1)/num_sides);
    float z2 = radius*sin(2*PI*(c1+1)/num_sides);
    float u1 = float(c1)/num_sides;
    float v1 = 1;
    float u2 = float(c1+1)/num_sides;
    float v2 = .5;
    std::vector<triangle_type> tmp = triangles_from_rectangle(rotate_point(make_tuple3(x1,0.0f,z1),rotation),rotate_point(make_tuple3(x2,0.0f,z2),rotation),rotate_point(make_tuple3(x1,magnitude,z1),rotation),rotate_point(make_tuple3(x2,magnitude,z2),rotation),u1,v1,u2,v2,random_color());
    for(int c2=0;c2<tmp.size();c2++)
    {
      obj->triangles.push_back(tmp[c2]);
    }
    u1 = .25;
    v1 = .25;
    u2 = u1+.25*x1/radius;
    v2 = v1+.25*z1/radius;
    float u3 = u1+.25*x2/radius;
    float v3 = v1+.25*z2/radius;
    obj->triangles.push_back(triangle_from_points(rotate_point(make_tuple3<float>(0,0,0),rotation),rotate_point(make_tuple3(x1,0.0f,z1),rotation),rotate_point(make_tuple3(x2,0.0f,z2),rotation),make_tuple3(make_tuple2(u1,v1),make_tuple2(u2,v2),make_tuple2(u3,v3)),random_color()));
    u1 = .75;
    v1 = .25;
    u2 = u1+.25*x1/radius;
    v2 = v1+.25*z1/radius;
    u3 = u1+.25*x2/radius;
    v3 = v1+.25*z2/radius;
    obj->triangles.push_back(triangle_from_points(rotate_point(make_tuple3<float>(0,magnitude,0),rotation),rotate_point(make_tuple3(x1,magnitude,z1),rotation),rotate_point(make_tuple3(x2,magnitude,z2),rotation),make_tuple3(make_tuple2(u1,v1),make_tuple2(u2,v2),make_tuple2(u3,v3)),random_color()));
  }
  obj->initialize_uvmap();
  return obj;
}

object3d* generate_sphereoid(unsigned int num_sides, unsigned int num_vert_segments, tuple3<float> radius,tuple4<float> color)
{
  object3d* obj = new object3d;
  bool use_rand_color = (color.z == 0);
  for(int c1=0;c1<num_sides;c1++)
  {
    for(int c2=0;c2<num_vert_segments;c2++)
    {
      float theta1 = 2*PI*float(c1)/num_sides;
      float theta2 = 2*PI*float(c1+1)/num_sides;
      float y_seg1 = PI*float(c2)/num_vert_segments;
      float y_seg2 = PI*float(c2+1)/num_vert_segments;
      float height1 = radius.y*cos(y_seg1);
      float height2 = radius.y*cos(y_seg2);
      tuple3<float> p1 = make_tuple3<float>(radius.x*sin(y_seg1)*cos(theta1),height1,radius.z*sin(y_seg1)*sin(theta1));
      tuple3<float> p2 = make_tuple3<float>(radius.x*sin(y_seg2)*cos(theta1),height2,radius.z*sin(y_seg2)*sin(theta1));
      tuple3<float> p3 = make_tuple3<float>(radius.x*sin(y_seg1)*cos(theta2),height1,radius.z*sin(y_seg1)*sin(theta2));
      tuple3<float> p4 = make_tuple3<float>(radius.x*sin(y_seg2)*cos(theta2),height2,radius.z*sin(y_seg2)*sin(theta2));
      if(use_rand_color)color = random_color();
      std::vector<triangle_type> tmp = triangles_from_rectangle(p1,p2,p3,p4,color);
      for(int c3=0;c3<tmp.size();c3++)
      {
        obj->triangles.push_back(tmp[c3]);
      }
    }
  }
  
  return obj;
}

object3d* combine_objects(std::vector<object3d*>& objects)
{
  object3d* obj = new object3d;
  while(objects.size() > 0)
  {
    while((objects.back())->triangles.size() > 0)
    {
      triangle_type tmp_tri = (objects.back())->triangles.back();
      for(int c2=0;c2<3;c2++)
      {
        tmp_tri.verts[c2].pos = rotate_point(tmp_tri.verts[c2].pos,(objects.back())->rotation);
        tmp_tri.verts[c2].pos = tmp_tri.verts[c2].pos + (objects.back())->position;
      }
      obj->triangles.push_back(tmp_tri);
      (objects.back())->triangles.pop_back();
    }
    delete objects.back();
    objects.pop_back();
  }
  return obj;
}

object3d* generate_spiral(unsigned int ngon_segments, unsigned int vert_segments, unsigned int height, float radius, float thickness, tuple4<float> color)
{
  std::vector<object3d*> objects;
  float deltay = float(height)/vert_segments;
  for(float y=0;y<height;y += deltay)
  {
    int circpoint = y/deltay;
    //for(int circpoint = 0;circpoint < ngon_segments;circpoint++)
    {
      tuple3<float> deltapoint = make_tuple3<float>(radius*(cos(2*PI*float(circpoint+1)/ngon_segments)-cos(2*PI*float(circpoint)/ngon_segments)),deltay,radius*(sin(2*PI*float(circpoint+1)/ngon_segments)-sin(2*PI*float(circpoint)/ngon_segments)));
      object3d* tmp = generate_ngon_tube(3,thickness,deltapoint,color);
      tmp->position = make_tuple3<float>(radius*cos(2*PI*float(circpoint)/ngon_segments),y,radius*sin(2*PI*float(circpoint)/ngon_segments));
      objects.push_back(tmp);
    }
  }
  return combine_objects(objects);
}

object3d* generate_dotted_spiral(unsigned int ngon_segments, unsigned int vert_segments, unsigned int height, float radius, float thickness, tuple4<float> color)
{
  std::vector<object3d*> objects;
  float deltay = float(height)/vert_segments;
  for(float y=0;y<height;y += deltay)
  {
    int circpoint = y/deltay;
    object3d* tmp = generate_sphereoid(3,2,make_tuple3<float>(thickness,thickness,thickness),color);
    tmp->position = make_tuple3<float>(radius*cos(2*PI*float(circpoint)/ngon_segments),y,radius*sin(2*PI*float(circpoint)/ngon_segments));
    objects.push_back(tmp);
  }
  return combine_objects(objects);
}

class opengl_panel : public Fl_Gl_Window
{
  public:
  int gl_mode;
  float MOVE_DELTA;
  float ROTATE_DELTA;
  tuple3<float> camera_pos;
  tuple3<float> camera_rot;
  std::vector<object3d*>* objects;
  char keybuffer[256];
  opengl_panel(int x, int y, int w, int h, const char* title=0) : Fl_Gl_Window(x,y,w,h,title)
  {
    objects = new std::vector<object3d*>;
    this->gl_mode = GL_TRIANGLES;
    camera_pos = make_tuple3<float>(0,0,0);
    camera_rot = make_tuple3<float>(0,0,0);
    MOVE_DELTA = .2;
    ROTATE_DELTA = .3;
  }
  ~opengl_panel()
  {
    if(objects != NULL)
    {
      while(objects->size() > 0)
      {
        delete ((*objects)[objects->size()-1]);
        objects->pop_back();
      }
      delete objects;
      objects = NULL;
    }
  }
  void draw()
  {
    if(objects == NULL)return;
    glViewport(0,0,this->w(),this->h());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1,1,-1,1,1,10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0,0,0,1);
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
		glRotatef(camera_rot.x,1,0,0);
		glRotatef(camera_rot.y,0,1,0);
		glRotatef(camera_rot.z,0,0,1);
		glTranslatef(-camera_pos.x,-camera_pos.y,-camera_pos.z);
		
    for(int c1=0;c1<objects->size();c1++)
    {
      if((*objects)[c1] == NULL)continue;
      if(!(((*objects)[c1])->visible))continue;
      if((*objects)[c1]->use_uvmap)
      {
		    (*objects)[c1]->uvmap->apply_texture();
      }
      else
      {
        glDisable(GL_TEXTURE_2D);
      }
      glBegin(gl_mode);
      glColor4f(1,1,1,1);
      for(int c2=0;c2<(*objects)[c1]->triangles.size();c2++)
      {
        for(int c3=0;c3<3;c3++)
        {
          vertex_type vert = (*objects)[c1]->triangles[c2].verts[c3];
          vert.pos = rotate_point(vert.pos,(*objects)[c1]->rotation);
          vert.pos = vert.pos + (*objects)[c1]->position;
          if(!(*objects)[c1]->use_uvmap)glColor4f(vert.color.w,vert.color.x,vert.color.y,vert.color.z);
          else glTexCoord2f(vert.texcoords.x,vert.texcoords.y);
          glVertex3f(vert.pos.x,vert.pos.y,vert.pos.z);
        }
      }
      glEnd();
    }
  }
  int handle(int event)
  {
    for(int c1=0;c1<256;c1++)
    {
      keybuffer[c1] = Fl::event_key(c1);
    }
    return Fl_Gl_Window::handle(event);
  }
};

int main(int argc, char* argv[])
{
  MTRand mtrandom;
  Fl_Window* window = new Fl_Window(WIDTH,HEIGHT,"Correspondence Problem Demonstration");
  opengl_panel* panel = new opengl_panel(0,0,WIDTH/2,HEIGHT);
  opengl_panel* panel2 = new opengl_panel(WIDTH/2,0,WIDTH/2,HEIGHT);
  window->end();
  window->show();
  
  delete panel2->objects;
  panel2->objects = panel->objects;
  
  //2 1 1 0 50 400
  if(argc == 1)
  {
    printf("Usage: %s rotate_speed show_barberpole show_spiral show_dotted_spiral spiral_sides spiral_vsegs whichtexture background_objects\nRunning without all specified uses defaults for remainder\nDefaults are 1,1,0,0,50,400,1,0\n",argv[0]);
  }
  float rotate_speed = (argc >= 2) ? .005*absolute(atoi(argv[1])) : .005;
  rotate_speed = (argc >= 2) ? ((absolute(atoi(argv[1]))==atoi(argv[1]))? rotate_speed : -rotate_speed) :rotate_speed;
  int show_barberpole = (argc >= 3) ? atoi(argv[2]) : 1;
  int show_spiral = (argc >= 4) ? atoi(argv[3]) : 0;
  int show_dotted_spiral = (argc >= 5) ? atoi(argv[4]) : 0;
  int spiral_sides = (argc >= 6) ? atoi(argv[5]) : 50;
  int spiral_vsegs = (argc >= 7) ? atoi(argv[6]) : 400;
  int whichtexture = (argc >= 8) ? atoi(argv[7]) : 1;
  int background_objects = (argc >= 9) ? atoi(argv[8]) : 0;
  for(int c1=0;c1<background_objects;c1++)
  {
    object3d* tmp = new object3d;
    tmp->position = make_tuple3(rand_float(-1000,1000),rand_float(-1000,1000),rand_float(-1000,1000));
    for(int c2=0;c2<5;c2++)
    {
      triangle_type tri;
      for(int c3=0;c3<3;c3++)
      {
        tri.verts[c3].pos = make_tuple3(rand_float(-50,50),rand_float(-50,50),rand_float(-50,50));
        tri.verts[c3].color = random_color();
      }
      tmp->triangles.push_back(tri);
    }
    panel->objects->push_back(tmp);
    tmp = generate_ngon_prism(mtrandom.randInt(5)+3,rand_float(10,100),rand_float(10,100));
    tmp->position = make_tuple3(rand_float(-2000,2000),rand_float(-2000,2000),rand_float(-2000,2000));
    panel->objects->push_back(tmp);
  }
  
  //good pairs for (spiral_sides,spiral_vsegs) include {(3,40), (50,40), (50,400)}
  if(show_spiral)panel->objects->push_back(generate_spiral(spiral_sides,spiral_vsegs,400,50,1,make_tuple4<float>(0,1,1,1)));
  if(show_dotted_spiral)panel->objects->push_back(generate_dotted_spiral(spiral_sides,spiral_vsegs,400,50,2,make_tuple4<float>(1,0,0,1)));
  int numsides = 16;
  if(show_barberpole)
  {
    panel->objects->push_back(generate_ngon_prism_uv(numsides,10,make_tuple3<float>(0,400,0)));
    switch(whichtexture)
    {
      case 0:
      panel->objects->back()->draw_uvmap_outline();
      break;
      case 1:
      panel->objects->back()->draw_uvmap_barberpole();
      break;
      case 2:
      panel->objects->back()->uvmap->texture_from_gimp(gimp_image);
      break;
      default:
      break;
    }
  }
  panel->camera_pos.y = panel2->camera_pos.y = 200;
  panel->camera_pos.z = panel2->camera_pos.z = 100;
  
  object3d* cylinder = NULL;
#define SHOW_CYLINDER 0
  object3d* sphere = generate_sphereoid(10,10,make_tuple3<float>(10,10,10),make_tuple4<float>(0,1,1,0));
  panel->objects->push_back(sphere);
  object3d* xaxis = generate_ngon_prism(3,1,make_tuple3<float>(100,0,0),make_tuple4<float>(1,0,0,1));//generate_sphereoid(8,4,make_tuple3<float>(100,10,10),make_tuple4<float>(1,0,0,1));
  object3d* yaxis = generate_ngon_prism(3,1,make_tuple3<float>(0,100,0),make_tuple4<float>(0,1,0,1));//generate_sphereoid(8,4,make_tuple3<float>(10,100,10),make_tuple4<float>(0,1,0,1));
  object3d* zaxis = generate_ngon_prism(3,1,make_tuple3<float>(0,0,100),make_tuple4<float>(0,0,1,1));//generate_sphereoid(8,4,make_tuple3<float>(10,10,100),make_tuple4<float>(0,0,1,1));
  panel->objects->push_back(xaxis);
  panel->objects->push_back(yaxis);
  panel->objects->push_back(zaxis);
  
  object3d* xyplane = generate_ngon_prism(4,100,make_tuple3<float>(0,0,.1),make_tuple4<float>(1,1,0,.25));
  //panel->objects->push_back(xyplane);
  object3d* xzplane = generate_ngon_prism(4,100,make_tuple3<float>(0,.1,0),make_tuple4<float>(1,0,1,.25));
  panel->objects->push_back(xzplane);
  object3d* yzplane = generate_ngon_prism(4,100,make_tuple3<float>(.1,0,0),make_tuple4<float>(0,1,1,.25));
  //panel->objects->push_back(yzplane);
  
  int counter = 0;
  while(Fl::wait() != 0)
  {
    sphere->position = panel->camera_pos;
    sphere->rotation.x = panel->camera_rot.y*PI/180;
    sphere->rotation.y = 0;
    sphere->rotation.z = -panel->camera_rot.x*PI/180;
    xaxis->rotation = yaxis->rotation = zaxis->rotation = make_tuple3<float>(0,0,0);
    xyplane->rotation = xzplane->rotation = yzplane->rotation = make_tuple3<float>(0,0,0);
    if(SHOW_CYLINDER)
    {
      if(cylinder != NULL)
      {
        panel->objects->pop_back();
        delete cylinder;
      }
    cylinder = generate_ngon_prism(3,5,sphere->position,make_tuple4<float>(.75,.75,.75,1));
    panel->objects->push_back(cylinder);
  }
    counter = ++counter % 500;
    switch(counter%2)
    {
      case 0:
      {
        sphere->visible = false;
        if(cylinder != NULL)cylinder->visible = false;
        panel->redraw();
      }
      break;
      case 1:
      {
        sphere->visible = true;
        if(cylinder != NULL)cylinder->visible = true;
        panel2->redraw();
      }
      break;
      default:
      break;
    }
    for(int c1=0;c1<panel->objects->size();c1++)
    {
      //((*panel->objects)[c1])->use_uvmap = !((*panel->objects)[c1])->use_uvmap;
      ((*panel->objects)[c1])->rotation.x += rotate_speed;
      //((*panel->objects)[c1])->rotation.z += ((c1%2)?0:1)*.005;
      //if((*panel->objects)[c1]->use_uvmap)(*panel->objects)[c1]->draw_uvmap_outline();
    }
    float rot_radian = PI*panel->camera_rot.y/180;
    if(panel->keybuffer['a'] != 0)
    {
      panel->camera_pos.x -= panel->MOVE_DELTA*cos(rot_radian);
      panel->camera_pos.z -= panel->MOVE_DELTA*sin(rot_radian);
    }
    if(panel->keybuffer['d'] != 0)
    {
      panel->camera_pos.x += panel->MOVE_DELTA*cos(rot_radian);
      panel->camera_pos.z += panel->MOVE_DELTA*sin(rot_radian);
    }
    if(panel->keybuffer['q'] != 0)panel->camera_pos.y -= panel->MOVE_DELTA;
    if(panel->keybuffer['e'] != 0)panel->camera_pos.y += panel->MOVE_DELTA;
    if(panel->keybuffer['w'] != 0)
    {
      panel->camera_pos.x -= panel->MOVE_DELTA*cos(rot_radian+(PI/2));
      panel->camera_pos.z -= panel->MOVE_DELTA*sin(rot_radian+(PI/2));
    }
    if(panel->keybuffer['s'] != 0)
    {
      panel->camera_pos.x += panel->MOVE_DELTA*cos(rot_radian+(PI/2));
      panel->camera_pos.z += panel->MOVE_DELTA*sin(rot_radian+(PI/2));
    }
    
    if(panel->keybuffer['k'] != 0)panel->camera_rot.x += panel->ROTATE_DELTA;
    if(panel->keybuffer['i'] != 0)panel->camera_rot.x -= panel->ROTATE_DELTA;
    if(panel->keybuffer['l'] != 0)panel->camera_rot.y += panel->ROTATE_DELTA;
    if(panel->keybuffer['j'] != 0)panel->camera_rot.y -= panel->ROTATE_DELTA;
    while(panel->camera_rot.x > 90)panel->camera_rot.x -= panel->ROTATE_DELTA;
    while(panel->camera_rot.x < -90)panel->camera_rot.x += panel->ROTATE_DELTA;
    if(panel->camera_rot.y > 360)panel->camera_rot.y = 0;
    if(panel->camera_rot.y < 0)panel->camera_rot.y = 360;
  }
  
  return 0;
}

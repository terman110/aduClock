#ifndef __AC_GEOMETRY_H__
#define __AC_GEOMETRY_H__

class acGeometry
{
public:
  const static int ElementWidth = 8;
  const static int ElementHeight = 8;
  const static int ElementsNX = 3;
  const static int ElementsNY = 1;
  const static int Width = ElementsNX * ElementWidth;
  const static int Height = ElementsNY * ElementHeight;
};

#endif // __AC_GEOMETRY_H__

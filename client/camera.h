#ifndef CAMERA_H
#define CAMERA_H

class Camera {
private:
  float x, y;
  int width, height;
  float zoom;

public:
  Camera();
  float getX() const;
  float getY() const;
  int getWidth() const;
  int getHeight() const;
  float getZoom() const;
  void setZoom(float zoom);
  void setPosition(float x, float y);
  void setView(float x, float y, int width, int height);

};


#endif //CAMERA_H

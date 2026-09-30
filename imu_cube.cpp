#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <array>
#include "AltIMU10.h"
#include "MadgwickAHRS.h"

// ---------- Quaternion utilities ----------
struct Quat {
  double w{1}, x{0}, y{0}, z{0};
};

static Quat normalize(const Quat& q) {
  double n = std::sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
  if (n <= 0) return {1,0,0,0};
  return {q.w/n, q.x/n, q.y/n, q.z/n};
}

static std::array<float, 16> quatToMat4(const Quat& q_in) {
  Quat q = normalize(q_in);
  const double w = q.w, x = q.x, y = q.y, z = q.z;
  const double xx = x*x, yy = y*y, zz = z*z;
  const double xy = x*y, xz = x*z, yz = y*z;
  const double wx = w*x, wy = w*y, wz = w*z;

  double r00 = 1.0 - 2.0*(yy + zz);
  double r01 = 2.0*(xy - wz);
  double r02 = 2.0*(xz + wy);
  double r10 = 2.0*(xy + wz);
  double r11 = 1.0 - 2.0*(xx + zz);
  double r12 = 2.0*(yz - wx);
  double r20 = 2.0*(xz - wy);
  double r21 = 2.0*(yz + wx);
  double r22 = 1.0 - 2.0*(xx + yy);

  std::array<float,16> m{};
  m[0]=(float)r00; m[4]=(float)r01; m[8]=(float)r02; m[12]=0.0f;
  m[1]=(float)r10; m[5]=(float)r11; m[9]=(float)r12; m[13]=0.0f;
  m[2]=(float)r20; m[6]=(float)r21; m[10]=(float)r22; m[14]=0.0f;
  m[3]=0.0f; m[7]=0.0f; m[11]=0.0f; m[15]=1.0f;
  return m;
}

// ---------- Global state ----------
static Quat g_q;
static int g_width = 900, g_height = 600;
static AltIMU10 imu;

static void drawAxes(float len=1.2f) {
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(len,0,0);
  glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,len,0);
  glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,len);
  glEnd();
}

static void drawCubeWire(float s=1.0f) {
  glColor3f(1,1,1);
  glutWireCube(s);
}

static void drawCubeSolid(float s=1.0f) {
  glColor3f(0.7f, 0.7f, 0.9f);
  glutSolidCube(s);
}

static void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 3.0,  0, 0, 0,  0, 1, 0);
  drawAxes();

  auto M = quatToMat4(g_q);
  glPushMatrix();
  glMultMatrixf(M.data());
  drawAxes(0.9f);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);
  drawCubeSolid(1.0f);
  glDisable(GL_POLYGON_OFFSET_FILL);

  drawCubeWire(1.01f);
  glPopMatrix();

  glutSwapBuffers();
}

static void reshape(int w, int h) {
  g_width = w; g_height = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (h>0)? (double)w/(double)h : 1.0, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

static void keyboard(unsigned char key, int, int) {
  if (key == 27 || key == 'q') std::exit(0);
}


static void timerFunc(int value) {
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  imu.readAccelGyro(ax, ay, az, gx, gy, gz);
  imu.readMag(mx, my, mz);

  MadgwickAHRSupdate(gx, gy, gz, ax, ay, az, mx, my, mz);

  g_q.w = q0; g_q.x = q1; g_q.y = q2; g_q.z = q3;

  glutPostRedisplay();
  glutTimerFunc(38, timerFunc, 0); 
}

static void initGL() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
}

int main(int argc, char** argv) {
  imu.init();
  g_q = {1,0,0,0};

  std::cout << "IMU-driven Cube Viewer\n"
            << "Press q or ESC to quit\n";

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(g_width, g_height);
  glutCreateWindow("IMU Quaternion Cube Orientation");

  initGL();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(38, timerFunc, 0);

  glutMainLoop();
  return 0;
}

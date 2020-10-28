#include <iostream>
#include <fstream>
#include <GLinclude.h>
#include <GLFW/glfw3.h>
//#include <GL/glu.h>
#include <CGL_FrameBuffer.h>
#include <CGL_Freetype.h>
#include <NPng16.h>

static CGL_Freetype StaticFreetype;
static CGL_FrameBuffer StaticFrameBuffer;

//typedef std::basic_string<unsigned char> ustring;

static unsigned char StaticAlphabet[256];
//const static int WindowWidth  = 1024;
//const static int WindowHeight = 1024;

using namespace std;

/*
 * static void DrawRect()
 * {
 *  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
 *
 *  glMatrixMode(GL_PROJECTION);
 *  glLoadIdentity();
 *  glOrtho(0, WindowWidth,0, WindowHeight, 0, 1);
 *
 *  glBegin(GL_QUADS);
 *  glVertex2f(100, 100);
 *  glVertex2f(200, 100);
 *  glVertex2f(200, 200);
 *  glVertex2f(100, 200);
 *  glEnd();
 * }
 */



//file:///home/steen/sdb2/MinGW/msys/1.0/home/steen/CreateFontTableau-Source/svn/CreateFontTableau/bin-windows/Alphabet.txt

#define ALPHABET    "-.,;[\\]\"$%$&{}:=_|<>* ?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789(/)'°@ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ¿"

static void CreateAlphabet()
{
    int j = 0;

    for (int i = 32; i < 128; i++)
    {
        StaticAlphabet[j++] = i;
    }

    for (int i = 128; i < 165; i++)
    {
        StaticAlphabet[j++] = i;
    }
}


int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        GLFWwindow* window;

        /* Initialize the library */
        const char* FontFile = argv[1];
        int Fontsize = NStringTool::Cast<int>(argv[2]);
        int TabSize = NStringTool::Cast<int>(argv[3]);
        int WindowWidth = TabSize;
        int WindowHeight = TabSize;

        CreateAlphabet();

        if (!glfwInit())
        {
            return -1;
        }
#if 1
        /* Create a windowed mode window and its OpenGL context */
        glfwWindowHint(GLFW_VISIBLE, 0);
        window = glfwCreateWindow(WindowWidth, WindowHeight, "Create Font Tableau OpenGL", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
#endif

        /* Make the window's context current */
        glfwMakeContextCurrent(window);


        glewInit();
        StaticFrameBuffer.CreateFramebufferObject(WindowWidth, WindowHeight);


        StaticFreetype.Init(FontFile, Fontsize);
        StaticFreetype.SetRenderModeMonochrome(true);


        bool once = false;
        CRectT<float>* rc = new CRectT<float>[256];
        memset(rc, 0, sizeof(CRectT<float>) * 256);

        std::string BaseName = NStringTool::FileBasename(argv[1]) + +"_" + argv[2];
        std::string RectFile = BaseName + ".rects";
        std::string BinFile = BaseName + ".bin";
        std::string TableauFile = BaseName + ".png";

        cout << "TableauFile=" << TableauFile << endl;


        /* Loop until the user closes the window */
        //while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            StaticFrameBuffer.DrawToFrameBuffer(true);

            glClear(GL_COLOR_BUFFER_BIT);

            //DrawRect();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, WindowWidth, 0, WindowHeight, 0, 1);
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            StaticFreetype.RenderStringRects(0, 0, WindowWidth, WindowHeight, (const unsigned char*)ALPHABET, rc);

            if (!once)
            {
                {
                    ofstream outfile(RectFile.c_str(), ios::binary);
                    for (int i = 0; i < 256; i++)
                    {
                        outfile.write((const char*)&rc[i].left, sizeof(float));
                        outfile.write((const char*)&rc[i].top, sizeof(float));
                        outfile.write((const char*)&rc[i].right, sizeof(float));
                        outfile.write((const char*)&rc[i].bottom, sizeof(float));
                    }
                    outfile.close();
                }

                {
                    ofstream outfile(BinFile.c_str(), ios::binary);
                    for (int i = 0; i < 256; i++)
                    {
                        CRectT<int> ri;
                        ri.left = rc[i].left   * WindowWidth;
                        ri.right = rc[i].right  * WindowWidth;
                        ri.top = rc[i].top    * WindowHeight;
                        ri.bottom = rc[i].bottom * WindowHeight;

                        outfile.write((const char*)&ri.left, sizeof(int));
                        outfile.write((const char*)&ri.top, sizeof(int));
                        outfile.write((const char*)&ri.right, sizeof(int));
                        outfile.write((const char*)&ri.bottom, sizeof(int));
                    }
                    outfile.close();
                }


                once = true;

                CDataRect dr;
                StaticFrameBuffer.ReadBuffer(&dr);

                dr.Yflip();
                NPng16::png_write(TableauFile.c_str(), dr);
            }


            StaticFrameBuffer.DrawToFrameBuffer(false);
            /* Swap front and back buffers */
            //glfwSwapBuffers(window);



            /* Poll for and process events */
            //	glfwPollEvents();
        }
        //glfwTerminate();
    }
    else
    {
        cout << "usage: ./createfonttableau_opengl <fontfile.ttf> <fontsize> <tableausize>" << endl;
        cout << "version 1.1" << endl;
    }


    return 0;
}

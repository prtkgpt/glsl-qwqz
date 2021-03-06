// test impl


#include "opengles_bridge.h"
#include "libqwqz.h"
#include "impl_main.h"
#include "test_main.h"
#include <spine/spine.h>
#include "spine_bridge.h"


static qwqz_handle qwqz_engine = NULL;
static float verticeBuffer[8];
static float uvBuffer[8];


int impl_draw() {
  qwqz_tick_timer(&qwqz_engine->m_Timers[0]);

  // not needed explicitly given that doShaderBg draws the to the entire screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, qwqz_engine->FramebufferName);
  glViewport(0, 0, qwqz_engine->m_RenderTextureWidth, qwqz_engine->m_RenderTextureWidth); // Render on the whole framebuffer, complete from the lower left corner to the upper right

  glUseProgram(qwqz_engine->m_Linkages[0].m_Program);
  //glUniform2f(qwqz_engine->m_Linkages[0].g_ResolutionUniform, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);
  glUniform1f(qwqz_engine->m_Linkages[0].g_TimeUniform, qwqz_engine->m_Timers[0].m_SimulationTime);

  //translate(&qwqz_engine->m_Linkages[0], NULL, 0, 0, 0);

  {
  qwqz_batch_prepare(qwqz_engine, &qwqz_engine->m_Batches[0]);
  size_t size_of_sprite = sizeof(struct qwqz_sprite_t);
  glVertexAttribPointer(qwqz_engine->m_Linkages[0].g_PositionAttribute, 2, GL_SHORT, GL_FALSE, size_of_sprite, (char *)NULL + (0));
  glEnableVertexAttribArray(qwqz_engine->m_Linkages[0].g_PositionAttribute);
  glVertexAttribPointer(qwqz_engine->m_Linkages[0].g_TextureAttribute, 2, GL_FLOAT, GL_FALSE, size_of_sprite, (char *)NULL + (2 * sizeof(GLshort)));
  glEnableVertexAttribArray(qwqz_engine->m_Linkages[0].g_TextureAttribute);
  }

  qwqz_batch_render(qwqz_engine, &qwqz_engine->m_Batches[0]);

  // Render to the screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);

  glUseProgram(qwqz_engine->m_Linkages[1].m_Program);
  //glUniform2f(qwqz_engine->m_Linkages[1].g_ResolutionUniform, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);
  glUniform1f(qwqz_engine->m_Linkages[1].g_TimeUniform, qwqz_engine->m_Timers[0].m_SimulationTime);

  //translate(&qwqz_engine->m_Linkages[1], NULL, 0, 0, 0);

  {
  qwqz_batch_prepare(qwqz_engine, &qwqz_engine->m_Batches[1]);
  size_t size_of_sprite = sizeof(struct qwqz_sprite_t);
  glVertexAttribPointer(qwqz_engine->m_Linkages[1].g_PositionAttribute, 2, GL_SHORT, GL_FALSE, size_of_sprite, (char *)NULL + (0));
  glEnableVertexAttribArray(qwqz_engine->m_Linkages[1].g_PositionAttribute);
  glVertexAttribPointer(qwqz_engine->m_Linkages[1].g_TextureAttribute, 2, GL_FLOAT, GL_FALSE, size_of_sprite, (char *)NULL + (2 * sizeof(GLshort)));
  glEnableVertexAttribArray(qwqz_engine->m_Linkages[1].g_TextureAttribute);
  }

  qwqz_batch_render(qwqz_engine, &qwqz_engine->m_Batches[1]);

  //qwqz_checkgl("main\n");

  return 0;
}


int impl_resize(int width, int height) {

  int resized = qwqz_resize(qwqz_engine, width, height);

    qwqz_batch_clear(&qwqz_engine->m_Batches[0]);
    qwqz_batch_clear(&qwqz_engine->m_Batches[1]);

verticeBuffer[0] = -qwqz_engine->m_ScreenHalfWidth;
verticeBuffer[1] = -qwqz_engine->m_ScreenHalfHeight;

verticeBuffer[2] = -qwqz_engine->m_ScreenHalfWidth;
verticeBuffer[3] = qwqz_engine->m_ScreenHalfHeight;

verticeBuffer[4] = qwqz_engine->m_ScreenHalfWidth;
verticeBuffer[5] = qwqz_engine->m_ScreenHalfHeight;

verticeBuffer[6] = qwqz_engine->m_ScreenHalfWidth;
verticeBuffer[7] = -qwqz_engine->m_ScreenHalfHeight;

uvBuffer[0] = 0.0;
uvBuffer[1] = 0.0;
uvBuffer[2] = 0.0;
uvBuffer[3] = 0.0;
uvBuffer[4] = 0.0;
uvBuffer[5] = 0.0;
uvBuffer[6] = 0.0;
uvBuffer[7] = 0.0;

    qwqz_batch_add(&qwqz_engine->m_Batches[0], 0, verticeBuffer, NULL, uvBuffer);
    qwqz_batch_add(&qwqz_engine->m_Batches[1], 0, verticeBuffer, NULL, uvBuffer);

    for (int i=0; i<2; i++) {
      glUseProgram(qwqz_engine->m_Linkages[i].m_Program);
      glUniform2f(qwqz_engine->m_Linkages[i].g_ResolutionUniform, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);
      qwqz_linkage_resize(&qwqz_engine->m_Linkages[i]);
    }
    
    glUseProgram(0);

    //glUniform2f(qwqz_engine->m_Linkages[2].g_ResolutionUniform, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);
    //glUniform2f(qwqz_engine->m_Linkages[1].g_ResolutionUniform, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);
    //glUniform2f(qwqz_engine->m_Linkages[0].g_ResolutionUniform, qwqz_engine->m_ScreenWidth, qwqz_engine->m_ScreenHeight);

  return resized;
}

/*
int impl_resize(int width, int height) {
  int resized = qwqz_resize(qwqz_engine, width, height);

  qwqz_batch_clear(&qwqz_engine->m_Batches[0]);
  qwqz_batch_clear(&qwqz_engine->m_Batches[1]);

  verticeBuffer[0] = -qwqz_engine->m_ScreenHalfWidth;
  verticeBuffer[1] = -qwqz_engine->m_ScreenHalfHeight;

  verticeBuffer[2] = -qwqz_engine->m_ScreenHalfWidth;
  verticeBuffer[3] = qwqz_engine->m_ScreenHalfHeight;

  verticeBuffer[4] = qwqz_engine->m_ScreenHalfWidth;
  verticeBuffer[5] = qwqz_engine->m_ScreenHalfHeight;

  verticeBuffer[6] = qwqz_engine->m_ScreenHalfWidth;
  verticeBuffer[7] = -qwqz_engine->m_ScreenHalfHeight;

  uvBuffer[0] = 0.0;
  uvBuffer[1] = 0.0;
  uvBuffer[2] = 0.0;
  uvBuffer[3] = 0.0;
  uvBuffer[4] = 0.0;
  uvBuffer[5] = 0.0;
  uvBuffer[6] = 0.0;
  uvBuffer[7] = 0.0;

  qwqz_batch_add(&qwqz_engine->m_Batches[0], 0, verticeBuffer, NULL, uvBuffer);
  qwqz_batch_add(&qwqz_engine->m_Batches[1], 0, verticeBuffer, NULL, uvBuffer);

  return resized;
}
*/


int impl_main(int argc, char** argv) {
  if (argc == 3) {
    qwqz_engine = qwqz_create();

    qwqz_engine->m_RenderTextureWidth = 512;

    GLuint v = 0;
    GLuint f = 0;
    GLuint f2 = 0;
    GLuint program = 0;

    qwqz_engine->m_Timers = (struct qwqz_timer_t *)malloc(sizeof(struct qwqz_timer_t) * 1);
    qwqz_timer_init(&qwqz_engine->m_Timers[0]);

    // render target
    int renderBufferTexture = qwqz_buffer_texture_init(GL_TEXTURE0);
    qwqz_engine->FramebufferName = qwqz_buffer_target_init(renderBufferTexture);

    qwqz_engine->m_Linkages = (struct qwqz_linkage_t *)malloc(sizeof(struct qwqz_linkage_t) * 2);

    v = qwqz_compile(GL_VERTEX_SHADER, argv[1]);
    f = qwqz_compile(GL_FRAGMENT_SHADER, argv[2]);
    f2 = qwqz_compile(GL_FRAGMENT_SHADER, "assets/shaders/test_background.fsh");

    if (v && f && f2) {
      // Create and link the shader program
      program = glCreateProgram();
      glAttachShader(program, v);
      glAttachShader(program, f);
      qwqz_linkage_init(program, &qwqz_engine->m_Linkages[0]);

      program = glCreateProgram();
      glAttachShader(program, v);
      glAttachShader(program, f2);
      qwqz_linkage_init(program, &qwqz_engine->m_Linkages[1]);
    }

    qwqz_engine->m_Batches = (struct qwqz_batch_t *)malloc(sizeof(struct qwqz_batch_t) * 2);

    qwqz_batch_init(&qwqz_engine->m_Batches[0], &qwqz_engine->m_Linkages[0], 1);
    qwqz_batch_init(&qwqz_engine->m_Batches[1], &qwqz_engine->m_Linkages[1], 1);
  }

  qwqz_checkgl("main\n");

  return 0;
}

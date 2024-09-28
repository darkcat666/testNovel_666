LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# sample.so
LOCAL_MODULE    := sample

# include
LOCAL_C_INCLUDES += ./
LOCAL_C_INCLUDES += ./impl
LOCAL_C_INCLUDES += ./gl-shared
LOCAL_C_INCLUDES += ./gl-shared/support

# compile
LOCAL_SRC_FILES    += ./gl-shared/GLApplication.c
LOCAL_SRC_FILES    += ./gl-shared/SampleList.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter01/sample_skelton.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter10/sample_blend_order.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter10/sample_blend_order_rev.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter10/sample_depth_depthbufferenable.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter10/sample_depth_order.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter10/sample_depth_order_rev.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube_datastruct.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube_vertexstruct.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube_vertexstruct_alignment.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube_vertexstruct_alignment_ex.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube_vertexstruct_indices.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_cube_vertexstruct_indices_degenerate.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_camera_setup.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter11/sample_rectangle_indices_degenerate.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_alpha.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_alpha2pass.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_edge.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_facechange.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_load.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_multirender.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_multirender_vbo.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter12/sample_pmd_rendering_highp.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter14/sample_pmd_glfinish.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter14/sample_pmd_glflush.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter15/sample_pmd_framebuffer.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter15/sample_pmd_framebuffer_alpha.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter15/sample_pmd_framebuffer_depth.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter15/sample_pmd_framebuffer_depth_notsupport.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter15/sample_pmd_framebuffer_depthshadow.c
LOCAL_SRC_FILES    += ./gl-shared/samples/chapter16/sample_async_load.c
LOCAL_SRC_FILES    += ./gl-shared/support/support.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_CompressedTexture_KtxImage.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_CompressedTexture_PkmImage.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_CompressedTexture_PvrtcImage.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_Pmd.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_Shader.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_Sprite.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_Texture.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_Texture_RawPixelImage.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_gl_Vector.c
LOCAL_SRC_FILES    += ./gl-shared/support/support_RawData.c
LOCAL_SRC_FILES    += ./impl/ES20_impl.c
LOCAL_SRC_FILES    += ./impl/ES20App_impl.c
LOCAL_SRC_FILES    += ./impl/NDKApplication_impl.c
LOCAL_SRC_FILES    += ./impl/RawData_impl.c
LOCAL_SRC_FILES    += ./impl/RawPixelImage_impl.c
LOCAL_SRC_FILES    += ./support_ndk.c

# libs
LOCAL_LDLIBS += -lGLESv2
LOCAL_LDLIBS += -llog

include $(BUILD_SHARED_LIBRARY)

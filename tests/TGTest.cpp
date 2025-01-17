#include "../../includes/TerreateGraphics.hpp"
#include "TerreateCore/math.hpp"

#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

using namespace TerreateGraphics::Core;
using namespace TerreateGraphics::Compute;
// using namespace TerreateMath::Utils;

struct Uniform {
  mat4 transform;
  mat4 model;
  mat4 view;
  mat4 proj;
};

struct Settings {
public:
  vec4 color;
  float a;

private:
  float padding[3];

public:
  vec4 dummy;

  Settings() : color(1, 1, 1, 1), a(0.5f), dummy(0, 0, 0, 1) {}
  Settings(vec4 const &color, float const &a, vec4 const &dummy)
      : color(color), a(a), dummy(dummy) {}
};

void OutputJoystickData(Joystick const &joystick, Text &text, Uint const &width,
                        Uint const &height) {
  JoystickAxisState axisState = joystick.AcquireAxisState();
  JoystickButtonState buttonState = joystick.AcquireButtonState();
  JoystickHatState hatState = joystick.AcquireHatState();

  Stream ss;
  if (joystick.IsGamepad()) {
    ss << "Gamepad: " << (Uint)joystick.GetJoystickId();
  } else {
    ss << "Joystick: " << (Uint)joystick.GetJoystickId();
  }

  if (!joystick.IsConnected()) {
    ss << " Disconnected";
    text.LoadText(ss.str());
    text.Render(0, 1500, width, height);
    return;
  } else {
    ss << " Connected";
    text.LoadText(ss.str());
    text.Render(0, 1500, width, height);
  }

  ss.str("");
  ss << "Left Stick: (" << std::fixed << std::setprecision(3)
     << axisState.leftStick[0] << ", " << axisState.leftStick[1] << ")";
  ss << " / Right Stick: (" << std::fixed << std::setprecision(3)
     << axisState.rightStick[0] << ", " << axisState.rightStick[1] << ")";
  ss << std::fixed << std::setprecision(3)
     << " / Left Trigger: " << axisState.leftTrigger
     << " / Right Trigger: " << axisState.rightTrigger;
  text.LoadText(ss.str());
  text.Render(0, 1450, width, height);

  ss.str("");
  ss << "A: " << buttonState.a << " / B: " << buttonState.b
     << " / X: " << buttonState.x << " / Y: " << buttonState.y;
  ss << " / Cross: " << buttonState.cross << " / Circle: " << buttonState.circle
     << " / Square: " << buttonState.square
     << " / Triangle: " << buttonState.triangle;
  text.LoadText(ss.str());
  text.Render(0, 1400, width, height);

  ss.str("");
  ss << "Left Bumper: " << buttonState.leftBumper
     << " / Right Bumper: " << buttonState.rightBumper
     << " / Back: " << buttonState.back << " / Start: " << buttonState.start
     << " / Guide: " << buttonState.guide
     << " / Left Thumb: " << buttonState.leftThumb
     << " / Right Thumb: " << buttonState.rightThumb;
  text.LoadText(ss.str());
  text.Render(0, 1350, width, height);

  ss.str("");
  ss << "D-Pad: Up: " << buttonState.dpadUp
     << " / Right: " << buttonState.dpadRight
     << " / Down: " << buttonState.dpadDown
     << " / Left: " << buttonState.dpadLeft;
  text.LoadText(ss.str());
  text.Render(0, 1300, width, height);

  ss.str("");
  ss << "Hat: Up: " << hatState.up << " / Right: " << hatState.right
     << " / Down: " << hatState.down << " / Left: " << hatState.left;
  text.LoadText(ss.str());
  text.Render(0, 1250, width, height);
}

class TestApp {
private:
  Clock mClock = Clock(120, 100);
  Shader mShader;
  Shader mScreenShader;
  mat4 mTransform;
  Float mWidth = 1500.0f;
  Float mHeight = 750.0f;
  Float mNear = 0.01f;
  Float mFar = 10000.0f;
  Float mDepth = mFar - mNear;

  Font mFont;
  Text mText;

  Font mInfoFont;
  Text mInfoText;

  Texture mTexture;
  Texture mTexture2;

  // WStr mTextString = L"日本語テスト";
  WStr mTextString = L"ABC";
  Uint mDelflag = 0u;
  Uint mPressingFlag = 0u;
  Uint mDelInterval = 10u;

  Buffer mBuffer;
  Buffer mScreenBuffer;
  BufferDataConstructor mColorDataConstructor;

  Screen mScreen;
  Uniform mUniform;
  Uniform mScreenUniform;
  UniformBuffer mUBO;
  UniformBuffer mScreenUBO;

  Settings mSettings;
  UniformBuffer mSettingsUniform;

public:
  void SizeCallback(Window *window, int const &width, int const &height) {
    glViewport(0, 0, width, height);
    mWidth = (Float)width;
    mHeight = (Float)height;
    mShader.Use();
    mUniform.transform = scale(
        identity<mat4>(), vec3(1.0f / mWidth, 1.0f / mHeight, 1.0f / mDepth));
    mUBO.ReloadData(mUniform);
  }

  void KeyCallback(Window *window, Key const &key) {
    if (key.key == Keyboard::K_ESCAPE) {
      window->Close();
    }
    if (key.key == Keyboard::K_BACKSPACE && key.action) {
      if (!mTextString.empty() &&
          (mDelflag > mDelInterval || mPressingFlag > 1)) {
        mDelflag = 0u;
        ++mPressingFlag;
        mTextString.pop_back();
        mText.LoadText(mTextString);
      }
    } else {
      mPressingFlag = 0u;
    }
  }

  void CharCallback(Window *window, Uint const &codepoint) {
    mTextString.push_back(codepoint);
    mText.LoadText(mTextString);
  }

public:
  TestApp() : mScreen(1000, 1000, 4) {
    mFont = Font("tests/resources/AsebiMin-Light.otf", 128);
    mInfoFont = Font("tests/resources/AsebiMin-Light.otf", 32);

    mText.LoadFont(&mFont);
    mText.LoadShader("tests/resources/shaders/textVert.glsl",
                     "tests/resources/shaders/textFrag.glsl");

    mInfoText.LoadFont(&mInfoFont);
    mInfoText.LoadShader("tests/resources/shaders/textVert.glsl",
                         "tests/resources/shaders/textFrag.glsl");

    mShader.AddVertexShaderSource(
        Shader::LoadShaderSource("tests/resources/shaders/mainVert.glsl"));
    mShader.AddFragmentShaderSource(
        Shader::LoadShaderSource("tests/resources/shaders/mainFrag.glsl"));
    mShader.Compile();
    mShader.Link();

    mScreenShader.AddVertexShaderSource(
        Shader::LoadShaderSource("tests/resources/shaders/screenVert.glsl"));
    mScreenShader.AddFragmentShaderSource(
        Shader::LoadShaderSource("tests/resources/shaders/screenFrag.glsl"));
    mScreenShader.Compile();
    mScreenShader.Link();

    Map<Str, Uint> attrs = {{"iPosition", 0}, {"iUV", 1}, {"iColor", 2}};
    BufferDataConstructor bdc;

    bdc.AddVertexComponent("iPosition", {{-600.0f, -600.0f, 600.0f},
                                         {600.0f, -600.0f, 600.0f},
                                         {600.0f, -600.0f, -600.0f},
                                         {-600.0f, -600.0f, -600.0f},
                                         {-600.0f, 600.0f, 600.0f},
                                         {600.0f, 600.0f, 600.0f},
                                         {600.0f, 600.0f, -600.0f},
                                         {-600.0f, 600.0f, -600.0f}});
    bdc.AddVertexComponent(
        "iUV", {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}});
    bdc.SetVertexIndices({
        {0, 0}, {1, 1}, {5, 2}, {4, 3}, {1, 0}, {2, 1}, {6, 2}, {5, 3},
        {2, 0}, {3, 1}, {7, 2}, {6, 3}, {3, 0}, {0, 1}, {4, 2}, {7, 3},
        {3, 0}, {2, 1}, {1, 2}, {0, 3}, {4, 0}, {5, 1}, {6, 2}, {7, 3},
    });
    bdc.Construct();
    mBuffer.LoadData(bdc, attrs);

    BufferDataConstructor screenBDC;
    screenBDC.AddVertexComponent("iPosition", {{-0.8f, -0.8f, 0.2f},
                                               {0.8f, -0.8f, 0.2f},
                                               {0.8f, 0.8f, 0.2f},
                                               {-0.8f, 0.8f, 0.2f}});
    screenBDC.AddVertexComponent(
        "iUV", {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}});
    screenBDC.SetVertexIndices({{0, 0}, {1, 1}, {2, 2}, {3, 3}});
    screenBDC.Construct();
    mScreenBuffer.LoadData(screenBDC, attrs);

    mColorDataConstructor.AddVertexComponent("iColor", {{1, 0, 0}});
    mColorDataConstructor.SetVertexIndices(
        {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
         {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}});
    mColorDataConstructor.Construct();
    mBuffer.LoadData(mColorDataConstructor, attrs);

    mBuffer.LoadIndices({{0, 1, 2, 2, 3, 0},
                         {4, 5, 6, 6, 7, 4},
                         {8, 9, 10, 10, 11, 8},
                         {12, 13, 14, 14, 15, 12},
                         {16, 17, 18, 18, 19, 16},
                         {20, 21, 22, 22, 23, 20}});
    mScreenBuffer.LoadIndices({0, 1, 2, 2, 3, 0});

    mat4 view = lookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 proj = perspective(45.0f, 1.0f, mNear, mFar);
    mUniform.view = view;
    mScreenUniform.view = identity<mat4>();
    mUniform.proj = proj;
    mScreenUniform.proj = identity<mat4>();
    mUniform.model = identity<mat4>();
    mScreenUniform.model = identity<mat4>();
    mUBO.LoadData(mUniform);
    mScreenUBO.LoadData(mScreenUniform);
    mUBO.Bind(mShader, "Matrices");
    mScreenUBO.Bind(mScreenShader, "Matrices");

    mSettings = {{1.0f, 1.0f, 1.0f, 1.0f}, 0.5f, {0, 0, 0, 1}};
    mSettingsUniform.LoadData(mSettings);
    mSettingsUniform.Bind(mShader, "Settings");

    // Uncomment if you want to break your brain...
    /* mShader.UseDepth(false); */

    mShader.Use();
    mShader.SetInt("uTexture", 0);
    Shader::ActivateTexture(TextureTargets::TEX_0);

    mScreenShader.Use();
    mScreenShader.SetInt("uTexture", 0);
    Shader::ActivateTexture(TextureTargets::TEX_0);

    mTexture2 = Texture(800, 800, 2);
    ImageConverter converter;
    Int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(true);
    Ubyte *pixels = stbi_load("tests/resources/testImage2.png", &width, &height,
                              &channels, 4);
    converter.Convert("testImage2", 1, width, height, channels, pixels,
                      mTexture2);
    stbi_image_free(pixels);
  }

  void OnFrame(Window *window) {
    window->PollEvents();
    window->Fill({0.2, 0.2, 0.2});
    window->Clear();

    /* auto state = */
    /*     Joystick::GetJoystick(JoystickID::JOYSTICK1).AcquireAxisState(); */
    /* Float angleX = state.leftStick[0]; */
    /* Float angleY = state.leftStick[1]; */
    /* mat4 model = rotate(identity<mat4>(), angleX, vec3(0, 1, 0)); */
    /* model = rotate(model, angleY, vec3(1, 0, 0)); */
    Float angle = mClock.GetCurrentRuntime() * 0.1;
    mat4 model = rotate(identity<mat4>(), angle, vec3(1, 1, 1));
    mUniform.model = model;
    mScreenUniform.model = model;
    mUBO.ReloadData(mUniform);
    mScreenUBO.ReloadData(mScreenUniform);

    Texture const &texture = mScreen.GetTexture();

    mScreen.Fill({0, 0, 0});
    mScreen.Clear();
    mScreen.Bind();
    mScreenShader.Use();
    /* mInfoFont.Use(); */
    mTexture2.Bind();
    mScreenBuffer.Draw(DrawMode::TRIANGLES);
    mTexture2.Unbind();
    /* mInfoFont.Unuse(); */
    mScreenShader.Unuse();
    /* mText.LoadText(L"立方体"); */
    /* auto size = mFont.AcquireTextSize(L"立方体"); */
    mText = mTextString;
    auto size = mFont.AcquireTextSize(mTextString);
    mText.Render(500 - size.first / 2.0, 500 - size.second / 2.0,
                 mScreen.GetWidth(), mScreen.GetHeight());
    mScreen.Unbind();

    mSettings = {{(std::sin(angle) + 1) / 2.0f, (std::cos(angle) + 1) / 2.0f,
                  1.0f, 1.0f},
                 0.5f,
                 {0, 0, 0, 1}};
    mSettingsUniform.ReloadData(mSettings);

    window->Bind();
    mShader.Use();
    texture.Bind();
    mBuffer.Draw(DrawMode::TRIANGLES);
    texture.Unbind();
    mShader.Unuse();

    mText = mTextString;
    mText.Render(0, 0, mWidth, mHeight);

    /* AttributeData color = mBuffer["iColor"]; */
    /* Float r = (state.leftTrigger + 1) / 2; */
    /* Float g = (state.rightTrigger + 1) / 2; */
    /* mColorDataConstructor.ReloadVertexComponent("iColor", {{r, g, 1}}); */
    /* mColorDataConstructor.Construct(); */
    /* mBuffer.ReloadData(color, mColorDataConstructor); */
    mText.SetColor({1, 0, 0});

    mInfoText = L"FPS: " + std::to_wstring(mClock.GetFPS());
    mInfoText.Render(0, 180, mWidth, mHeight);

    Joystick const &joystick = Joystick::GetJoystick(JoystickID::JOYSTICK1);
    OutputJoystickData(joystick, mInfoText, mWidth, mHeight);

    window->Swap();
    ++mDelflag;
    mClock.Tick();
  }
};

void TestCompute() {
  Vec<Float> inputData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  ShaderStorageBuffer input, input2, output, output2;
  input.LoadData(inputData);
  output.Allocate(input.GetSize());
  input2.LoadData(inputData);
  output2.Allocate(input2.GetSize());

  ComputeKernel kernel;
  kernel.AddKernelSource(
      Shader::LoadShaderSource("tests/resources/shaders/compute.glsl"));
  kernel.Compile();
  kernel.Link();

  input.Bind(kernel, "InputBuffer");
  output.Bind(kernel, "OutputBuffer");

  kernel.SetFloat("scaleFactor", 2.0f);
  kernel.Dispatch(10, 1, 1);

  ComputeKernel kernel2;
  kernel2.AddKernelSource(
      Shader::LoadShaderSource("tests/resources/shaders/compute.glsl"));
  kernel2.Compile();
  kernel2.Link();

  input2.Bind(kernel2, "InputBuffer");
  output2.Bind(kernel2, "OutputBuffer");

  kernel2.SetFloat("scaleFactor", 3.0f);
  kernel2.Dispatch(10, 1, 1);

  Vec<float> outputData, outputData2;
  output.GetData(outputData);
  output2.GetData(outputData2);
  for (int i = 0; i < inputData.size(); ++i) {
    std::cout << "output[" << i << "] = " << outputData[i] << std::endl;
  }
  for (int i = 0; i < inputData.size(); ++i) {
    std::cout << "output2[" << i << "] = " << outputData2[i] << std::endl;
  }
}

int main() {
  Initialize();
  {
    Window window(2500, 1600, "Test Window", WindowSettings());
    window.DisableVsync();

    TestApp app;
    window.GetSizeEventHandler().Subscribe(
        [&app](Window *window, int const &width, int const &height) {
          app.SizeCallback(window, width, height);
        });
    window.GetKeyEventHandler().Subscribe(
        [&app](Window *window, Key const &key) {
          app.KeyCallback(window, key);
        });
    window.GetCharEventHandler().Subscribe(
        [&app](Window *window, Uint const &codepoint) {
          app.CharCallback(window, codepoint);
        });

    // TestCompute();

    while (window) {
      window.Frame([&app](Window *window) { app.OnFrame(window); });
    }
  }
  Terminate();
}

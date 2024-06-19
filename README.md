# YZPost-Processing
Post-Processing shaders

This project created to test concepts and present results.

MyGame.cp file in TestGame folder implements game's main functionality as setup object, shaders etc.
main.cpp in the same folder starting game instance and run's it while game is working.

Shader files available in TestGame/Assets/Shaders folder

Rendering pass is located in YZ/src/Graphics/GraphicsEngine.cpp

Current demo preview:
![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/7124f9d9-ba2f-4c8e-81f7-e5bc6116bcee)

Also, here's some screenshots of game's post-process shaders:

Bright light with default post-process
![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/77fa0341-4e77-417d-bd4e-a41c7212fc66)

Bright light with HDR
![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/c5d2b31b-d812-416f-b0e4-e2bcccfd9aba)

Normal light
  Default post-process
  ![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/4fc7e48f-9111-47f9-96be-f2fecc5fcd1a)

  Chromatic abberation
  ![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/05440d99-f7f9-42d7-bc3b-87a735f6ff51)

  Vignette
  ![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/91469a82-2254-4b52-94fb-813f90938d89)

  Grayscale
  ![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/94104c8f-10db-4f06-a4b9-d0b59c1fd93c)

  Black and White
  ![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/f2c5826f-cf6c-4f97-84ac-a1748514a4e9)

  Also i was testing with Bloom, but that code was a mess. Heres result:
  ![image](https://github.com/ZennLuu/YZPost-Processing/assets/154216577/8f4f8dd2-bedc-42b9-8106-5871ef6d423e)
  Fps count here's down due to compiling shaders at each frame :) I was testing possibility to chain up different post-process shaders.

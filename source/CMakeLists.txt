if(NOT DEFINED RADIX_LIBRARIES)
  message(FATAL_ERROR "Please compile using the repo's root CMakeLists.txt")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXE_NAME}
  map/MapListLoader.cpp
  map/MapLoader.cpp
  map/XmlHelper.cpp
  material/MaterialLoader.cpp
  model/MeshLoader.cpp
  scene/Scene.cpp
  scene/Screen.cpp
  scene/Terminal.cpp
  shader/Shader.cpp
  shader/ShaderLoader.cpp
  text/FontLoader.cpp
  texture/TextureLoader.cpp
  Camera.cpp
  Camera.cpp
  component/Health.cpp
  component/Player.cpp
  component/RigidBody.cpp
  component/SoundSource.cpp
  component/Transform.cpp
  core/diag/AnsiConsoleLogger.cpp
  core/diag/Logger.cpp
  core/diag/LogInput.cpp
  core/diag/StdoutLogger.cpp
  core/event/Dispatcher.cpp
  core/event/Observer.cpp
  core/event/observer/FlagObserver.cpp
  core/event/observer/MusicObserver.cpp
  core/file/Path.cpp
  core/gl/TightDataPacker.cpp
  core/gl/VBO.cpp
  core/math/Math.cpp
  core/math/Matrix3f.cpp
  core/math/Matrix4f.cpp
  core/math/Vector2f.cpp
  core/math/Vector3f.cpp
  core/math/Vector4f.cpp
  core/state/GameState.cpp
  core/state/PlayerState.cpp
  EntityManager.cpp
  env/ArgumentsParser.cpp
  env/Config.cpp
  env/Environment.cpp
  env/System.cpp
  physics/CollisionDispatcher.cpp
  physics/KinematicCharacterController.cpp
  physics/PhysicsDebugDraw.cpp
  physics/PhysicsHelper.cpp
  physics/Uncollider.cpp
  renderer/GWENRenderer.cpp
  renderer/Renderer.cpp
  renderer/TerminalRenderer.cpp
  renderer/UiRenderer.cpp
  SoundManager.cpp
  system/PhysicsSystem.cpp
  system/PlayerSystem.cpp
  system/PortalSystem.cpp
)
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES})
install(TARGETS ${EXE_NAME} DESTINATION bin)
add_custom_target(update_assets COMMAND git submodule update --depth 1)
add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data DEPENDS ${EXE_NAME})
add_custom_target(maptest COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data --map test DEPENDS ${EXE_NAME} update_assets)
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
#depends on package similarity-tester
add_custom_target(cpd COMMAND sim_c -f -F ./*.cpp)

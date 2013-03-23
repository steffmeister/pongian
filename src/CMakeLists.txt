set ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} )

Find_Package ( SDL REQUIRED )
Find_Package ( SDL_image REQUIRED )
Find_Package ( SDL_mixer REQUIRED )
Find_Package ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL_ERROR "SDL not found!" )
	endif ( NOT SDL_FOUND )

include_directories (SFont yakwailog)

add_executable (../pongian pongian.cxx xtheme.cxx ball.cxx brick.cxx paddle.cxx SFont/SFont.c yakwailog/yakwailog.cxx)

target_link_libraries (
	../pongian
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDLMIXER_LIBRARY}
	${SDLTTF_LIBRARY}
	SDLmain
)

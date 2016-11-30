CC		= g++
C		= cpp
H       = h

CFLAGS	= -g 
LFLAGS	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lOpenImageIO -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS   = -L /usr/lib64/ -lglut -lGL -lGLU -lOpenImageIO -lm
  endif
endif

EXE			= map 
HFILES  = ImageIO/ImageIO.${H} ImageIO/Image.${H} utils.${H} 
OBJS    = ImageIO/ImageIO.o ImageIO/Image.o utils.o

all: ${EXE}

map: ${OBJS} map.o
	${CC} ${LFLAGS} -o map ${OBJS} map.o ${LDFLAGS}

map.o: map.${C} ${HFILES}
	${CC} ${CFLAGS} -c map.${C}

ImageIO.o: ImageIO/ImageIO.${C} ${HFILES}
	${CC} ${CFLAGS} -c ImageIO.${C} 

Image.o: ImageIO/Image.${C} ${HFILES}
	${CC} ${CFLAGS} -c Image.${C}

utils.o: utils.${C} ${HFILES}
	${CC} ${CFLAGS} -c utils.${C} 

clean:
	rm -f core.* *.o *~ ${EXE} ${OBJS}

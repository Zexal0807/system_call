CFLAGS   = -Wall -std=gnu99
INCLUDES = -I .
OBJDIR   = obj
PROCESSDIR = process

all: $(OBJDIR) R1 R2 R3 receiver_manager S1 S2 S3 sender_manager hackler

# Create object directory (with sub directory)
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/process
	@mkdir -p $(OBJDIR)/struct

DEFINES_SRCS = defines.c struct/process.c struct/child.c struct/message.c struct/trafficInfo.c struct/hacklerAction.c struct/list.c

# Define and create R1 executable
RECEIVER_1_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c process/R1.c
RECEIVER_1_OBJS = $(addprefix $(OBJDIR)/, $(RECEIVER_1_SRCS:.c=.o))

R1: $(RECEIVER_1_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create R2 executable
RECEIVER_2_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c process/R2.c
RECEIVER_2_OBJS = $(addprefix $(OBJDIR)/, $(RECEIVER_2_SRCS:.c=.o))

R2: $(RECEIVER_2_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create R3 executable
RECEIVER_3_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c process/R3.c
RECEIVER_3_OBJS = $(addprefix $(OBJDIR)/, $(RECEIVER_3_SRCS:.c=.o))

R3: $(RECEIVER_3_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create Receiver Manager
RECEIVER_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c receiver_manager.c
RECEIVER_OBJS = $(addprefix $(OBJDIR)/, $(RECEIVER_SRCS:.c=.o))

receiver_manager: $(RECEIVER_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create S1 executable
SENDER_1_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c process/S1.c
SENDER_1_OBJS = $(addprefix $(OBJDIR)/, $(SENDER_1_SRCS:.c=.o))

S1: $(SENDER_1_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create S2 executable
SENDER_2_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c process/S2.c
SENDER_2_OBJS = $(addprefix $(OBJDIR)/, $(SENDER_2_SRCS:.c=.o))

S2: $(SENDER_2_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create S3 executable
SENDER_3_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c process/S3.c
SENDER_3_OBJS = $(addprefix $(OBJDIR)/, $(SENDER_3_SRCS:.c=.o))

S3: $(SENDER_3_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create Sender Manager
SENDER_SRCS = $(DEFINES_SRCS) err_exit.c shared_memory.c semaphore.c pipe.c fifo.c sender_manager.c
SENDER_OBJS = $(addprefix $(OBJDIR)/, $(SENDER_SRCS:.c=.o))

sender_manager: $(SENDER_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

# Define and create Hackler
HACKLER_SRCS = $(DEFINES_SRCS) err_exit.c semaphore.c hackler.c
HACKLER_OBJS = $(addprefix $(OBJDIR)/, $(HACKLER_SRCS:.c=.o))

hackler: $(HACKLER_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@  -lm

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

run:
	./sender_manager input/F0.csv & ./receiver_manager & ./hackler input/F7.csv

# Remove all object files
clean:
	@rm -vf ${RECEIVER_1_OBJS}
	@rm -vf R1
	@rm -vf ${RECEIVER_2_OBJS}
	@rm -vf R2
	@rm -vf ${RECEIVER_3_OBJS}
	@rm -vf R3
	@rm -vf ${RECEIVER_OBJS}
	@rm -vf receiver_manager

	@rm -vf ${SENDER_1_OBJS}
	@rm -vf S1
	@rm -vf ${SENDER_2_OBJS}
	@rm -vf S2
	@rm -vf ${SENDER_3_OBJS}
	@rm -vf S3
	@rm -vf ${SENDER_OBJS}
	@rm -vf sender_manager

	@rm -vf ${HACKLER_OBJS}
	@rm -vf hackler

	@rm -rf ${OBJDIR}
	@ipcrm -a

	@rm -f output/*
	@echo "Removed object files and executables..."

.PHONY: run clean


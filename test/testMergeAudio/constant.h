#pragma once

#define FILENAME1		("./Test1.raw")
#define FILENAME2		("./Test2.raw")
#define	NUM_CHANNELS	(2)
#define PA_TYPE			(paInt16)
#define	SAMPLE_RATE		(24000)
#define FRAMES_PER_BUFFER	(480)
#define SIZE_PER_SECOND	(SAMPLE_RATE * NUM_CHANNELS)
#define RECORDING_SECOND		(1)
#define RECORDING_BUFF_SIZE	(SIZE_PER_SECOND * RECORDING_SECOND)
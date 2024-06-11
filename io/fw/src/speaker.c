/*
 * Author: Pawel Jablonski
 * E-mail: pj@xirx.net
 * WWW: xirx.net
 * GIT: git.xirx.net
 *
 * License: You can use this code however you like
 * but leave information about the original author.
 * Code is free for non-commercial and commercial use.
 */

#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <stm32f0xx_hal_dac.h>
#include <stm32f0xx_hal_tim.h>
#include <stm32f0xx_hal_dma.h>
#include <stm32f0xx_hal_rcc.h>
#include <system_stm32f0xx.h>

#include "type.h"

#include "speaker.h"

#define SPEAKER_BUFFER_SIZE 32 //!< Note buffer size
#define SPEAKER_BUFFER_OUT_SIZE 16 //!< Output buffer size and square wave fill divider

#define SPEAKER_NOTE_QUANTITY 89 //!< Note quantity available to play

#define SPEAKER_TIME_MIN 1 //!< Minimum time to play for a note in ms multiplied by TIME_MUL
#define SPEAKER_TIME_MAX 250 //!< Maximum time to play for a note in ms multiplied by TIME_MUL
#define SPEAKER_TIME_MUL 10 //!< Time multiplicator for a note

#define SPEAKER_FILL_MIN 1 //!< Minimum fill of the square wave in percents divided by SPEAKER_BUFFER_OUT_SIZE
#define SPEAKER_FILL_MAX 15 //!< Maximum fill of the square wave in percents divided by SPEAKER_BUFFER_OUT_SIZE

#define SPEAKER_VOLUME_MAX 15.0 //!< Maximum volume level

#define SPEAKER_VALUE_ZERO 2048 //!< Output zero level for silence
#define SPEAKER_VALUE_MUL 2047.0 //!< Output level multiplicator used to conversion
#define SPEAKER_VALUE_VOLUME 0.1 //!< Output volume level multiplicator

//! List of frequencies for notes
static const unsigned short noteFrequency[SPEAKER_NOTE_QUANTITY] = {
	20, // Silent
	27, 29, 31, //  A0 - B0
	33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, // C1 - B1
	65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123, // C2 - B2
	131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, // C3 - B3
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, // C4 - B4
	523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, // C5 - B5
	1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, // C6 - B6
	2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, // C7 - B7
	4186 // C8
};

static volatile bool isPlaying = FALSE; //!< Playing status

static volatile unsigned int bufferStart = 0; //!< Data start position
static volatile unsigned int bufferEnd = 0; //!< Data end position

static DAC_HandleTypeDef dacHandle; //!< DAC HAL handle
static TIM_HandleTypeDef timHandle; //!< Timer HAL handle for playing square wave
static DAC_ChannelConfTypeDef channelConf; //!< Configuration of DAC HAL
static DMA_HandleTypeDef dmaHandle; //!< DMA HAL handle
static TIM_HandleTypeDef timNoteHandle; //!< Timer HAL handle for making a delay between notes

static struct SpeakerNote buffer[SPEAKER_BUFFER_SIZE]; //!< Circular note buffer

static uint16_t bufferOut[SPEAKER_BUFFER_OUT_SIZE]; //!< Output buffer

static void speakerPlayNextNote();

//! Init speaker and set up DMA, DAC, Timers
void speakerInit(void)
{
	for(int i = 0; i < SPEAKER_BUFFER_OUT_SIZE; i++)
	{
		bufferOut[i] = SPEAKER_VALUE_ZERO;
	}
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInit = {0};
	
	gpioInit.Pin = GPIO_PIN_4;
	gpioInit.Mode = GPIO_MODE_ANALOG;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &gpioInit);
	
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	timHandle.Instance = TIM2;
	timHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	timHandle.Init.Prescaler = 0;
	timHandle.Init.Period = ((HAL_RCC_GetHCLKFreq() / 100000) - 1);
	timHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timHandle.Init.RepetitionCounter = 0;
	
	HAL_TIM_Base_Init(&timHandle);
	HAL_TIM_Base_Start_IT(&timHandle);
	
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&timHandle, &sMasterConfig);
	
	__HAL_RCC_DAC1_CLK_ENABLE();
	
	dacHandle.Instance = DAC;
	
	HAL_DAC_Init(&dacHandle);
	
	channelConf.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
    channelConf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	
	HAL_DAC_ConfigChannel(&dacHandle, &channelConf, DAC_CHANNEL_1);
	HAL_DAC_Start(&dacHandle, DAC_CHANNEL_1);
	
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	dmaHandle.Instance = DMA1_Channel3;
	dmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	dmaHandle.Init.MemInc = DMA_MINC_ENABLE;
	dmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	dmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	dmaHandle.Init.Mode = DMA_CIRCULAR;
	dmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
	
	HAL_DMA_Init(&dmaHandle);
	
	__HAL_LINKDMA(&dacHandle, DMA_Handle1, dmaHandle);
	
	HAL_DAC_Start_DMA(&dacHandle, DAC_CHANNEL_1, (uint32_t *)bufferOut, SPEAKER_BUFFER_OUT_SIZE, DAC_ALIGN_12B_R);
	
	__HAL_RCC_TIM6_CLK_ENABLE();
	
	timNoteHandle.Instance = TIM6;
	timNoteHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	timNoteHandle.Init.Prescaler = ((HAL_RCC_GetHCLKFreq() / 1000) - 1);
	timNoteHandle.Init.Period = 0;
	timNoteHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timNoteHandle.Init.RepetitionCounter = 0;
	
	HAL_TIM_Base_Init(&timNoteHandle);
	
	HAL_NVIC_SetPriority(TIM6_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
}

//! Start playing sound from the buffer
void speakerPlay(void)
{
	__disable_irq();
	if(isPlaying == FALSE)
	{
		speakerPlayNextNote();
	}
	__enable_irq();
}

//! Stop playing sound
void speakerPause(void)
{
	__disable_irq();
	if(isPlaying == TRUE)
	{
		isPlaying = FALSE;
		
		HAL_TIM_Base_Stop_IT(&timNoteHandle);
		
		for(int i = 0; i < SPEAKER_BUFFER_OUT_SIZE; i++)
		{
			bufferOut[i] = SPEAKER_VALUE_ZERO;
		}
	}
	__enable_irq();
}

//! Clear the speaker buffer and stop playing sound
void speakerClear(void)
{
	__disable_irq();
	{
		isPlaying = FALSE;
		
		bufferStart = 0;
		bufferEnd = 0;
		
		HAL_TIM_Base_Stop_IT(&timNoteHandle);
	
		for(int i = 0; i < SPEAKER_BUFFER_OUT_SIZE; i++)
		{
			bufferOut[i] = SPEAKER_VALUE_ZERO;
		}
	}
	__enable_irq();
}

/**
 * Check if sound is playing
 *
 * @return Status of playing of sound
 */
bool speakerIsPlaying(void)
{
	return(isPlaying);
}

/**
 * Add note to the buffer
 *
 * @param note Note to add to the buffer
 */
void speakerAddNote(struct SpeakerNote note)
{
	if(note.note >= SPEAKER_NOTE_QUANTITY)
	{
		return;
	}
	
	if((note.time < SPEAKER_TIME_MIN) || (note.time > SPEAKER_TIME_MAX))
	{
		return;
	}
	
	if(note.note != NOTE_SILENT)
	{
		if((note.fill < SPEAKER_FILL_MIN) || (note.fill > SPEAKER_FILL_MAX))
		{
			return;
		}
		
		if(note.volume > SPEAKER_VOLUME_MAX)
		{
			return;
		}
	}
	
	__disable_irq();
	{
		unsigned int last = bufferEnd;
		
		if(bufferStart == ((last + 1) % SPEAKER_BUFFER_SIZE))
		{
			__enable_irq();
			return;
		}
		
		buffer[last].note = note.note;
		buffer[last].time = note.time;
		
		if(note.note == NOTE_SILENT)
		{
			buffer[last].fill = SPEAKER_FILL_MIN;
			buffer[last].volume = 0;
		}
		else
		{
			buffer[last].fill = note.fill;
			buffer[last].volume = note.volume;
		}
		
		bufferEnd = ((last + 1) % SPEAKER_BUFFER_SIZE);
	}
	__enable_irq();
	
	return;
}

/**
 * Get free space size in the buffer
 *
 * @return Free space size in the buffer
 */
unsigned char speakerGetBufferFree(void)
{
	unsigned char bufferFree = (SPEAKER_BUFFER_SIZE - 1);
	
	__disable_irq();
	{
		if(bufferStart < bufferEnd)
		{
			bufferFree -= (bufferEnd - bufferStart);
		}
		else if(bufferStart > bufferEnd)
		{
			bufferFree = ((bufferStart - bufferEnd) - 1);
		}	
	}
	__enable_irq();
	
	return(bufferFree);
}

//! Play a next note
static void speakerPlayNextNote(void)
{
	if(bufferStart == bufferEnd)
	{
		speakerClear();
	}
	else
	{
		uint16_t signalLevel = (uint16_t)(SPEAKER_VALUE_MUL * SPEAKER_VALUE_VOLUME * ((float)(buffer[bufferStart].volume) / SPEAKER_VOLUME_MAX));
		
		uint16_t high = (SPEAKER_VALUE_ZERO + signalLevel);
		uint16_t low = (SPEAKER_VALUE_ZERO - signalLevel);
		
		for(int i = 0; i < SPEAKER_BUFFER_OUT_SIZE; i++)
		{
			bufferOut[i] = ((i < buffer[bufferStart].fill) ? high : low);
		}
		
		__HAL_TIM_SET_AUTORELOAD(&timHandle, ((HAL_RCC_GetHCLKFreq() / (noteFrequency[buffer[bufferStart].note] * SPEAKER_BUFFER_OUT_SIZE)) - 1));
		__HAL_TIM_SET_COUNTER(&timHandle, 0);
		
		__HAL_TIM_SET_AUTORELOAD(&timNoteHandle, (((uint32_t)(buffer[bufferStart].time) * SPEAKER_TIME_MUL) - 1));
		__HAL_TIM_SET_COUNTER(&timNoteHandle, 0);
		
		if(!isPlaying)
		{
			isPlaying = TRUE;
			
			__HAL_TIM_CLEAR_FLAG(&timNoteHandle, TIM_FLAG_UPDATE);
			HAL_TIM_Base_Start_IT(&timNoteHandle);
		}
		
		bufferStart = ((bufferStart + 1) % SPEAKER_BUFFER_SIZE);
	}
}

//! Load and play a next note from the note buffer. It is an IRQ handler connected to the timer HAL handle for making a delay between notes.
void TIM6_IRQHandler(void)
{
	if(isPlaying == TRUE)
	{
		speakerPlayNextNote();
	}
	
	HAL_TIM_IRQHandler(&timNoteHandle);	
}

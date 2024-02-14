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

#include "cpu.h"

/**
 * Constructor for the CPU class
 *
 * @param parent Parent object
 */
CPU::CPU(QObject *parent) : QObject(parent)
{
	this->urom0.data.fill(0);
	this->urom1.data.fill(0);
	this->bios.data.fill(0);
	this->ram.data.fill(0);

	this->reset();

	QObject::connect(&this->timer, SIGNAL(timeout()), this, SLOT(emulation()));
}

/**
 * Set first uROM memory data
 *
 * @param urom uROM data
 */
void CPU::setUrom0(const CPU::UROM &urom)
{
	this->urom0 = urom;
}

/**
 * Set second uROM memory data
 *
 * @param urom uROM data
 */
void CPU::setUrom1(const CPU::UROM &urom)
{
	this->urom1 = urom;
}

/**
 * Set BIOS memory data
 *
 * @param bios BIOS data
 */
void CPU::setBios(const CPU::BIOS &bios)
{
	this->bios = bios;
}

//! Run executing emulation
void CPU::run()
{
	this->stepMode = false;

	this->timer.setSingleShot(false);
	this->timer.setInterval(INTERVAL);
	this->timer.start();
}

//! Run only one CPU step of emulation
void CPU::step()
{
	this->stepMode = true;

	this->timer.setSingleShot(true);
	this->timer.start(0);
}

//! Pause executing emulation
void CPU::pause()
{
	this->timer.stop();
}

//! Stop executing emulation
void CPU::stop()
{
	this->timer.stop();

	this->reset();
}

/**
 * Get register buffer
 *
 * @return Register buffer
 */
const CPU::Reg &CPU::getReg() const
{
	return(this->reg);
}

/**
 * Get ticks counter value
 *
 * @return Tick counter value
 */
unsigned long long CPU::getTicks() const
{
	return(this->ticks);
}

/**
 * Get ROM buffer
 *
 * @return ROM buffer
 */
const CPU::BIOS &CPU::getBios() const
{
	return(this->bios);
}

/**
 * Get Memory buffer
 *
 * @return Memory buffer
 */
const CPU::RAM &CPU::getRam() const
{
	return(this->ram);
}

//! Stop executing and reset data of emulation
void CPU::reset()
{
	this->stepMode = false;
	this->ticks = 0;

	this->timer.stop();

	this->ram.data.fill(0);

	this->reg.i = 0;
	this->reg.c.fill(false);
	this->reg.z.fill(false);
	this->reg.pch = 0;
	this->reg.pcl = 0;
	this->reg.sph = static_cast<unsigned char>(MEMORY_SP_ADDRESS >> 8);
	this->reg.spl = 0;
	this->reg.maxSp = 0;
	this->reg.a = 0;
	this->reg.b = 0;
	this->reg.x = 0;
	this->reg.y = 0;
	this->reg.in = 0;
	this->reg.out = 0;
	this->reg.d = 0;
	this->reg.t = 0;
	this->reg.mah = 0;
	this->reg.mal = 0;
}

/**
 * Compute logic or arithmetic using ALU. It emulates functions of the 74181 chip.
 *
 * @param a First operant
 * @param b Second operant
 * @param s Operation type selector in range 0-15
 * @param m Operatiom mode selector. "0" for arithmetic and "1" for logic mode.
 * @param c Carry flag
 * @param z Zero flag
 *
 * @return Computed value
 */
unsigned char CPU::alu(unsigned char a, unsigned char b, unsigned char s, bool m, bool &c, bool &z)
{
	QVector<bool> inA(4);
	QVector<bool> inB(4);
	QVector<bool> inS(4);

	inA[0] = ((a & (1 << 0)) != 0);
	inA[1] = ((a & (1 << 1)) != 0);
	inA[2] = ((a & (1 << 2)) != 0);
	inA[3] = ((a & (1 << 3)) != 0);

	inB[0] = ((b & (1 << 0)) != 0);
	inB[1] = ((b & (1 << 1)) != 0);
	inB[2] = ((b & (1 << 2)) != 0);
	inB[3] = ((b & (1 << 3)) != 0);

	inS[0] = ((s & (1 << 0)) != 0);
	inS[1] = ((s & (1 << 1)) != 0);
	inS[2] = ((s & (1 << 2)) != 0);
	inS[3] = ((s & (1 << 3)) != 0);

	QVector<QVector<bool>> l1(4, QVector<bool>(5));

	for(int i = 0; i < 4; i++)
	{
		l1[i][0] = inA[i];
		l1[i][1] = (inB[i] & inS[0]);
		l1[i][2] = ((!inB[i]) & inS[1]);
		l1[i][3] = ((!inB[i]) & inA[i] & inS[2]);
		l1[i][4] = (inA[i] & inB[i] & inS[3]);
	}

	QVector<QVector<bool>> l2(4, QVector<bool>(2));

	for(int i = 0; i < 4; i++)
	{
		l2[i][0] = !(l1[i][0] | l1[i][1] | l1[i][2]);
		l2[i][1] = !(l1[i][3] | l1[i][4]);
	}

	QVector<bool> l3(20);

	l3[0] = !(c & (!m));

	l3[1] = !(l2[0][0]);
	l3[2] = ((!m) & l2[0][0]);
	l3[3] = ((!m) & l2[0][1] & c);

	l3[4] = !(l2[1][0]);
	l3[5] = ((!m) & l2[1][0]);
	l3[6] = ((!m) & l2[0][0] & l2[1][1]);
	l3[7] = ((!m) & l2[0][1] & l2[1][1] & c);

	l3[8] = !(l2[2][0]);
	l3[9] = ((!m) & l2[2][0]);
	l3[10] = ((!m) & l2[1][0] & l2[2][1]);
	l3[11] = ((!m) & l2[0][0] & l2[1][1] & l2[2][1]);
	l3[12] = ((!m) & l2[0][1] & l2[1][1] & l2[2][1] & c);

	l3[13] = !(l2[3][0]);
	l3[14] = !(l2[0][1] & l2[1][1] & l2[2][1] & l2[3][1]);
	l3[15] = !(l2[0][1] & l2[1][1] & l2[2][1] & l2[3][1] & c);

	l3[16] = (l2[0][0] & l2[1][1] & l2[2][1] & l2[3][1]);
	l3[17] = (l2[1][0] & l2[2][1] & l2[3][1]);
	l3[18] = (l2[2][0] & l2[3][1]);
	l3[19] = (l2[3][0]);

	QVector<bool> l4(8);

	l4[0] = (l3[1] & l2[0][1]);
	l4[1] = !(l3[2] | l3[3]);

	l4[2] = (l3[4] & l2[1][1]);
	l4[3] = !(l3[5] | l3[6] | l3[7]);

	l4[4] = (l3[8] & l2[2][1]);
	l4[5] = !(l3[9] | l3[10] | l3[11] | l3[12]);

	l4[6] = (l3[13] & l2[3][1]);
	l4[7] = !(l3[16] | l3[17] | l3[18] | l3[19]);

	QVector<bool> f(4);

	f[0] = (l3[0] ^ l4[0]);
	f[1] = (l4[1] ^ l4[2]);
	f[2] = (l4[3] ^ l4[4]);
	f[3] = (l4[5] ^ l4[6]);

	z = (f[0] & f[1] & f[2] & f[3]);
	c = ((!l3[15]) | (!l4[7]));

	unsigned char out = 0;

	out += (f[0] ? 1 : 0);
	out += (f[1] ? 2 : 0);
	out += (f[2] ? 4 : 0);
	out += (f[3] ? 8 : 0);

	return(out);
}

/**
 * Set Input register value
 *
 * @param in Input register value
 */
void CPU::inSlot(unsigned char in)
{
	if(this->ticks > 0)
	{
		this->reg.in = ((this->reg.in & CLOCK_TICKS_IN_MASK) | (in & ~CLOCK_TICKS_IN_MASK));
	}
}

//! Process one step of the emulation
void CPU::emulation()
{
	unsigned long long tick = 0;

	unsigned char uromCycle = 0;
	int uromAddress;
	unsigned char urom0;
	unsigned char urom1;

	BusAR busAR;
	BusAW busAW;
	BusB busB;
	BusC busC;

	int address;

	unsigned char aluS;
	bool aluM;
	bool aluC;

	unsigned char busARregABXY;
	unsigned char busAWregABXY;
	unsigned char busBregAB;

	unsigned char valueAR = 0;
	unsigned char valueB = 0;

	while(tick < TICKS_PER_INTERVAL)
	{
		do
		{
			bool &regC = ((this->reg.i & INSTRUCTION_REG_CZ_SELECT_MASK) ? this->reg.c[1] : this->reg.c[0]);
			bool &regZ = ((this->reg.i & INSTRUCTION_REG_CZ_SELECT_MASK) ? this->reg.z[1] : this->reg.z[0]);

			uromAddress = (static_cast<int>(this->reg.i) << UROM_ADDRESS_INSTRUCTION_POSITION);
			uromAddress += (static_cast<int>(uromCycle & UROM_ADDRESS_CYCLE_MASK) << UROM_ADDRESS_CYCLE_POSITION);
			uromAddress += (static_cast<int>(regC ? 1 : 0) << UROM_ADDRESS_FLAG_C_POSITION);
			uromAddress += (static_cast<int>(regZ ? 1 : 0) << UROM_ADDRESS_FLAG_Z_POSITION);

			urom0 = this->urom0.data[uromAddress];
			urom1 = this->urom1.data[uromAddress];

			uromCycle++;

			busAR = static_cast<BusAR>((urom0 & UROM_0_BUS_AR_MASK) >> UROM_0_BUS_AR_POSITION);
			busAW = static_cast<BusAW>((urom0 & UROM_0_BUS_AW_MASK) >> UROM_0_BUS_AW_POSITION);
			busB = static_cast<BusB>((urom0 & UROM_0_BUS_B_MASK) >> UROM_0_BUS_B_POSITION);
			busC = static_cast<BusC>((urom1 & UROM_1_BUS_C_MASK) >> UROM_1_BUS_C_POSITION);

			aluS = ((urom1 & UROM_1_ALU_S_MASK) >> UROM_1_ALU_S_POSITION);
			aluM = ((urom1 & UROM_1_ALU_M_MASK) != 0);
			aluC = ((urom1 & UROM_1_ALU_C_MASK) != 0);

			if((busAR == BusAR::ABXY) && (busAW == BusAW::ABXY))
			{
				busARregABXY = ((this->reg.i & INSTRUCTION_REG_ABXY_SECOND_MASK) >> INSTRUCTION_REG_ABXY_SECOND_OFFSET);
			}
			else
			{
				busARregABXY = ((this->reg.i & INSTRUCTION_REG_ABXY_FIRST_MASK) >> INSTRUCTION_REG_ABXY_FIRST_OFFSET);
			}

			busAWregABXY = (this->reg.i & INSTRUCTION_REG_ABXY_FIRST_MASK);
			busBregAB = ((this->reg.i & INSTRUCTION_REG_AB_MASK) >> INSTRUCTION_REG_AB_OFFSET);

			switch(busC)
			{
				case BusC::PC :
					address = ((static_cast<int>(this->reg.pch) << 8) + static_cast<int>(this->reg.pcl));
					break;

				case BusC::MA :
					address = ((static_cast<int>(this->reg.mah) << 8) + static_cast<int>(this->reg.mal));
					break;

				case BusC::SP :
					address = ((static_cast<int>(this->reg.sph) << 8) + static_cast<int>(this->reg.spl));
					break;

				case BusC::XY :
					address = ((static_cast<int>(this->reg.y) << 8) + static_cast<int>(this->reg.x));
					break;
			}

			switch(busAR)
			{
				case BusAR::ABXY :
					switch(busARregABXY)
					{
						case 0 :
							valueAR = this->reg.a;
							break;

						case 1 :
							valueAR = this->reg.b;
							break;

						case 2 :
							valueAR = this->reg.x;
							break;

						case 3 :
							valueAR = this->reg.y;
							break;
					}
					break;

				case BusAR::D :
					valueAR = this->reg.d;
					break;

				case BusAR::IN :
					valueAR = this->reg.in;
					break;

				case BusAR::T :
					valueAR = this->reg.t;
					break;

				case BusAR::RAM :
					if(address < BIOS_SIZE)
					{
						valueAR = this->bios.data[address];
					}
					else
					{
						valueAR = this->ram.data[address];
					}
					break;

				case BusAR::PC :
					if(busB == BusB::Low)
					{
						valueAR = this->reg.pcl;
					}
					else
					{
						valueAR = this->reg.pch;
					}
					break;

				case BusAR::SP :
					if(busB == BusB::Low)
					{
						valueAR = this->reg.spl;
					}
					else
					{
						valueAR = this->reg.sph;
					}
					break;

				case BusAR::BP :
					if(busB == BusB::Low)
					{
						valueAR = this->reg.bpl;
					}
					else
					{
						valueAR = this->reg.bph;
					}
					break;
			}

			switch(busB)
			{
				case BusB::AB :
					switch(busBregAB)
					{
						case 0 :
							valueB = this->reg.a;
							break;

						case 1 :
							valueB = this->reg.b;
							break;
					}
					break;

				case BusB::D :
					valueB = this->reg.d;
					break;
			}

			switch(busAW)
			{
				case BusAW::None :
					break;

				case BusAW::ABXY :
					switch(busAWregABXY)
					{
						case 0 :
							this->reg.a = valueAR;
							break;

						case 1 :
							this->reg.b = valueAR;
							break;

						case 2 :
							this->reg.x = valueAR;
							break;

						case 3 :
							this->reg.y = valueAR;
							break;
					}
					break;

				case BusAW::D :
					this->reg.d = valueAR;
					break;

				case BusAW::OUT :
					{
						this->reg.out = valueAR;

						emit outSignal(this->reg.out);
					}
					break;

				case BusAW::ALU_T :
					{
						QVector<bool> z(2);
						bool c = aluC;
						unsigned char value = 0;

						value = this->alu((valueAR & ALU_4BIT_MASK), (valueB & ALU_4BIT_MASK), aluS, aluM, c, z[0]);
						value += (this->alu((valueAR >> ALU_4BIT_OFFSET), (valueB >> ALU_4BIT_OFFSET), aluS, aluM, c, z[1]) << ALU_4BIT_OFFSET);

						this->reg.t = value;
						regC = c;
						regZ = (z[0] & z[1]);
					}
					break;

				case BusAW::RPC :
					uromCycle = 0;
					break;

				case BusAW::I :
					this->reg.i = valueAR;
					break;

				case BusAW::RAM :
					if(address >= BIOS_SIZE)
					{
						this->ram.data[address] = valueAR;
					}
					break;

				case BusAW::PC :
					if(busB == BusB::Low)
					{
						this->reg.pcl = valueAR;
					}
					else
					{
						this->reg.pch = valueAR;
					}
					break;

				case BusAW::SP :
					if(busB == BusB::Low)
					{
						this->reg.spl = valueAR;
					}
					else
					{
						this->reg.sph = (static_cast<unsigned char>(MEMORY_SP_ADDRESS >> 8) | valueAR);
					}
					break;

				case BusAW::BP :
					if(busB == BusB::Low)
					{
						this->reg.bpl = valueAR;
					}
					else
					{
						this->reg.bph = valueAR;
					}
					break;

				case BusAW::MA :
					if(busB == BusB::Low)
					{
						this->reg.mal = valueAR;
					}
					else
					{
						this->reg.mah = valueAR;
					}
					break;

				case BusAW::PC_PLUS :
					{
						unsigned int pcAddress = ((static_cast<unsigned int>(this->reg.pch) << 8) + static_cast<unsigned int>(this->reg.pcl) + 1);

						this->reg.pch = static_cast<unsigned char>(pcAddress >> 8);
						this->reg.pcl = static_cast<unsigned char>(pcAddress & 0xff);
					}
					break;

				case BusAW::SP_PLUS :
					this->reg.spl++;
					this->reg.maxSp = qMax(this->reg.maxSp, ((static_cast<unsigned int>(this->reg.sph) << 8) + static_cast<unsigned int>(this->reg.spl)));
					break;

				case BusAW::SP_MINUS :
					this->reg.spl--;
					break;

				case BusAW::RPC_PLUS :
					{
						unsigned int pcAddress = ((static_cast<unsigned int>(this->reg.pch) << 8) + static_cast<unsigned int>(this->reg.pcl) + 1);

						this->reg.pch = static_cast<unsigned char>(pcAddress >> 8);
						this->reg.pcl = static_cast<unsigned char>(pcAddress & 0xff);

						uromCycle = 0;
					}
					break;
			}

			tick++;
			this->ticks++;

			if((this->ticks % CLOCK_TICKS_PER_INTERVAL) == 0)
			{
				this->reg.in ^= IO::IN_CLOCK_BIT;
			}
		}
		while(uromCycle > 0);

		if(this->stepMode)
		{
			break;
		}
	}

	emit updateSignal();
}

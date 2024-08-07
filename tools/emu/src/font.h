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

#ifndef FONT_H
#define FONT_H

#include <QVector>

//! 8x8 ASCII font definition
static const QVector<QVector<unsigned char>> fontTable = QVector<QVector<unsigned char>>({
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00 },
	{ 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00 },
	{ 0x30, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x30, 0x00 },
	{ 0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00 },
	{ 0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00 },
	{ 0x60, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00 },
	{ 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00 },
	{ 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00 },
	{ 0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60 },
	{ 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00 },
	{ 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00 },
	{ 0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0x7C, 0x00 },
	{ 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00 },
	{ 0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00 },
	{ 0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00 },
	{ 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00 },
	{ 0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00 },
	{ 0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00 },
	{ 0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00 },
	{ 0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00 },
	{ 0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00 },
	{ 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00 },
	{ 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60 },
	{ 0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00 },
	{ 0x00, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x00 },
	{ 0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00 },
	{ 0x78, 0xCC, 0x0C, 0x18, 0x30, 0x00, 0x30, 0x00 },
	{ 0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0x00 },
	{ 0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00 },
	{ 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00 },
	{ 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00 },
	{ 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00 },
	{ 0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00 },
	{ 0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00 },
	{ 0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00 },
	{ 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00 },
	{ 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },
	{ 0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00 },
	{ 0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00 },
	{ 0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00 },
	{ 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00 },
	{ 0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00 },
	{ 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00 },
	{ 0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00 },
	{ 0x78, 0xCC, 0xCC, 0xCC, 0xDC, 0x78, 0x1C, 0x00 },
	{ 0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00 },
	{ 0x78, 0xCC, 0x60, 0x30, 0x18, 0xCC, 0x78, 0x00 },
	{ 0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },
	{ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00 },
	{ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00 },
	{ 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00 },
	{ 0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00 },
	{ 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00 },
	{ 0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00 },
	{ 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00 },
	{ 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00 },
	{ 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00 },
	{ 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF },
	{ 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00 },
	{ 0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00 },
	{ 0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00 },
	{ 0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00 },
	{ 0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00 },
	{ 0x38, 0x6C, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0x00 },
	{ 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 },
	{ 0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00 },
	{ 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 },
	{ 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78 },
	{ 0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00 },
	{ 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },
	{ 0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xC6, 0x00 },
	{ 0x00, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00 },
	{ 0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00 },
	{ 0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0 },
	{ 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E },
	{ 0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0xF0, 0x00 },
	{ 0x00, 0x00, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x00 },
	{ 0x10, 0x30, 0x7C, 0x30, 0x30, 0x34, 0x18, 0x00 },
	{ 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00 },
	{ 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00 },
	{ 0x00, 0x00, 0xC6, 0xD6, 0xFE, 0xFE, 0x6C, 0x00 },
	{ 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00 },
	{ 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 },
	{ 0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00 },
	{ 0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00 },
	{ 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 },
	{ 0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00 },
	{ 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x18, 0x18, 0x3C, 0x3C, 0xA5, 0xE7, 0xFF, 0x99 },
	{ 0x3C, 0x7E, 0xFF, 0x99, 0xDB, 0x7E, 0xDB, 0xA5 },
	{ 0x00, 0x3C, 0x7E, 0x99, 0xFF, 0x3C, 0x5A, 0xA5 },
	{ 0x00, 0x00, 0x18, 0xBD, 0x7E, 0xDB, 0xFF, 0xC5 },
	{ 0x00, 0x07, 0x1F, 0x32, 0x7F, 0xFF, 0x39, 0x00 },
	{ 0x00, 0xE0, 0xF8, 0x4C, 0xFE, 0xFF, 0x9C, 0x00 },
	{ 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00 },
	{ 0x00, 0x42, 0x24, 0x00, 0x00, 0x24, 0x42, 0x00 },
	{ 0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00 },
	{ 0x00, 0x00, 0x7F, 0x80, 0xFF, 0x7F, 0x00, 0x00 },
	{ 0x00, 0x00, 0xFE, 0x01, 0xFF, 0xFE, 0x00, 0x00 },
	{ 0x38, 0x7C, 0xFE, 0xE0, 0xFE, 0x7C, 0x38, 0x00 },
	{ 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00 },
	{ 0x38, 0x7C, 0x92, 0xB6, 0xFE, 0xFE, 0xAA, 0x00 },
	{ 0x10, 0x30, 0x38, 0x10, 0x7C, 0x38, 0x38, 0x28 },
	{ 0x38, 0x6C, 0xD6, 0xD6, 0xD6, 0x6C, 0x38, 0x00 },
	{ 0xAA, 0xFF, 0x89, 0xFF, 0xA1, 0xFF, 0x89, 0xFF },
	{ 0x14, 0x08, 0x14, 0x20, 0x30, 0x78, 0x78, 0x30 },
	{ 0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00 },
	{ 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00 },
	{ 0x38, 0x7C, 0x38, 0xFE, 0xFE, 0xD6, 0x10, 0x38 },
	{ 0x10, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x10, 0x38 },
	{ 0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78 },
	{ 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18 },
	{ 0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C },
	{ 0x3C, 0x42, 0xA5, 0x81, 0x99, 0xA5, 0x42, 0x3C },
	{ 0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00 },
	{ 0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x00 },
	{ 0x00, 0x00, 0x3C, 0x24, 0x24, 0x3C, 0x00, 0x00 },
	{ 0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00 },
	{ 0xFE, 0xFE, 0xFE, 0xEE, 0xFE, 0xFE, 0xFE, 0x00 },
	{ 0xFE, 0xFE, 0xFE, 0xBA, 0xFE, 0xFE, 0xFE, 0x00 },
	{ 0xFE, 0xFA, 0xFE, 0xEE, 0xFE, 0xBE, 0xFE, 0x00 },
	{ 0xFE, 0xBA, 0xFE, 0xFE, 0xFE, 0xBA, 0xFE, 0x00 },
	{ 0xFE, 0xBA, 0xFE, 0xEE, 0xFE, 0xBA, 0xFE, 0x00 },
	{ 0xFE, 0xAA, 0xFE, 0xFE, 0xFE, 0xAA, 0xFE, 0x00 },
	{ 0x00, 0x00, 0x00, 0x38, 0x38, 0x10, 0x38, 0x7C },
	{ 0x10, 0x38, 0x38, 0x38, 0x10, 0x10, 0x38, 0x7C },
	{ 0x18, 0x2C, 0x7C, 0xEE, 0x4E, 0x1C, 0x3E, 0x7F },
	{ 0x6C, 0x7C, 0x7C, 0x38, 0x38, 0x38, 0x7C, 0xFE },
	{ 0x10, 0x54, 0x54, 0x38, 0xBA, 0x7C, 0x38, 0x7C },
	{ 0x6C, 0xFE, 0xD6, 0xD6, 0x7C, 0x38, 0x7C, 0x7C },
	{ 0x00, 0x18, 0x18, 0x7E, 0x7E, 0x18, 0x18, 0x00 },
	{ 0x00, 0x33, 0xCC, 0x00, 0x00, 0x33, 0xCC, 0x00 },
	{ 0xCC, 0x88, 0x33, 0x22, 0xCC, 0x88, 0x33, 0x22 },
	{ 0x10, 0x7C, 0xFE, 0xFE, 0xFE, 0x7C, 0x10, 0x38 },
	{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
	{ 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00 },
	{ 0x18, 0x18, 0x18, 0xF8, 0xF8, 0x00, 0x00, 0x00 },
	{ 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0x18 },
	{ 0x00, 0x00, 0x00, 0xF8, 0xF8, 0x18, 0x18, 0x18 },
	{ 0x18, 0x18, 0x18, 0xF8, 0xF8, 0x18, 0x18, 0x18 },
	{ 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00 },
	{ 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x18, 0x18, 0x18 },
	{ 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x18, 0x18, 0x18 },
	{ 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x18, 0x18, 0x18 },
	{ 0x18, 0x18, 0x38, 0xF0, 0xE0, 0x00, 0x00, 0x00 },
	{ 0x18, 0x18, 0x1C, 0x0F, 0x07, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1C, 0x18, 0x18 },
	{ 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x38, 0x18, 0x18 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00 },
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0 },
	{ 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 },
	{ 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0 },
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F },
	{ 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F },
	{ 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F },
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F },
	{ 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88 },
	{ 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
	{ 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77 },
	{ 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 },
	{ 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 },
	{ 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF },
	{ 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF },
	{ 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC },
	{ 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8 },
	{ 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 },
	{ 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0 },
	{ 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00 },
	{ 0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00 },
	{ 0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00 },
	{ 0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00 },
	{ 0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00 },
	{ 0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00 },
	{ 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00 },
	{ 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00 },
	{ 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00 },
	{ 0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0x00 },
	{ 0x3C, 0x66, 0xF0, 0x60, 0xF0, 0x66, 0x3C, 0x00 },
	{ 0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00 },
	{ 0xCC, 0xCC, 0x78, 0xFC, 0x30, 0xFC, 0x30, 0x30 },
	{ 0x7C, 0x82, 0x9A, 0xA2, 0xA2, 0x9A, 0x82, 0x7C },
	{ 0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00 },
	{ 0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0xCC, 0x78 },
	{ 0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00 },
	{ 0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0 },
	{ 0x10, 0x38, 0x6C, 0x6C, 0xC6, 0xC6, 0xFE, 0x00 },
	{ 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00 },
	{ 0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00 },
	{ 0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00 },
	{ 0x00, 0x0C, 0xFE, 0x18, 0x30, 0xFE, 0x60, 0x00 },
	{ 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0xFC, 0x00 },
	{ 0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00 },
	{ 0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00 },
	{ 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00 },
	{ 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00 },
	{ 0x30, 0x30, 0x00, 0xFC, 0x00, 0x30, 0x30, 0x00 },
	{ 0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00 },
	{ 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
});

#endif

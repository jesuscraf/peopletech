//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ttf2pfa.cpp
// Project:			 RFNBuilder
// Author:           Meta Software M.S. , S.A
// Date:			 20/01/2000
// Language:         C++
// Operating System: ALL
// Design Document:  
//
// Propietary:		Manuel Lazcano Perez
// Modifications:	Who && Date
//
// Definition: This module defines the conversion fron True Type Format
//		       files to PFA (Adobe Type 1) format files for Postscript.
//
//==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <io.h>
#include "rfn.h"
#include "rfndf.inl"
#include "ttf2pfa.h"
#include "sysinfo.h"
#include "rfnfilew.h"
#include "m4unicode.hpp"

#define GROUP_ID RFN_TGI_OTHERS

//===============================================================================

m4int_t UnicodeToWin31 (m4int_t ai_iUnival)
{
    if (ai_iUnival <= 0x0081) {
        return ai_iUnival;
    } else if (ai_iUnival >= 0x00a0 && ai_iUnival <= 0x00ff) {
        return ai_iUnival;
    } else {
        switch (ai_iUnival) {
            case 0x008d: return 0x8d;
            case 0x008e: return 0x8e;
            case 0x008f: return 0x8f;
            case 0x0090: return 0x90;
            case 0x009d: return 0x9d;
            case 0x009e: return 0x9e;
            case 0x0152: return 0x8c;
            case 0x0153: return 0x9c;
            case 0x0160: return 0x8a;
            case 0x0161: return 0x9a;
            case 0x0178: return 0x9f;
            case 0x0192: return 0x83;
            case 0x02c6: return 0x88;
            case 0x02dc: return 0x98;
            case 0x2013: return 0x96;
            case 0x2014: return 0x97;
            case 0x2018: return 0x91;
            case 0x2019: return 0x92;
            case 0x201a: return 0x82;
            case 0x201c: return 0x93;
            case 0x201d: return 0x94;
            case 0x201e: return 0x84;
            case 0x2020: return 0x86;
            case 0x2021: return 0x87;
            case 0x2022: return 0x95;
            case 0x2026: return 0x85;
            case 0x2030: return 0x89;
            case 0x2039: return 0x8b;
            case 0x203a: return 0x9b;
            case 0x2122: return 0x99;
            default: return 0xffff;
		};
    };
}

m4bool_t HandleName(StAuxParamsTTF *ao_pStAuxParams)
{
	m4char_t *pcNameBuffer = ao_pStAuxParams->m_pcNameBuffer;
	m4int_t iLanguage, iLength, iPlatform;
    m4char_t *pcString, *pcStringArea;
	m4bool_t SwOk = M4_TRUE;
    m4int_t iFound = 0;
	m4int_t j, k;

    pcStringArea = (char *)ao_pStAuxParams->m_NameTable + ntohs(ao_pStAuxParams->m_NameTable->m_Offset);
    ao_pStAuxParams->m_NameRecord = &(ao_pStAuxParams->m_NameTable->m_NameRecords);

    for (j=0; j<8; j++) {
        ao_pStAuxParams->m_pcNameFields[j] = NULL;
    };

    for (j=0; j < ntohs(ao_pStAuxParams->m_NameTable->m_NumberOfNameRecords); j++) {

        iPlatform = ntohs(ao_pStAuxParams->m_NameRecord->m_PlatformID);

        if (iPlatform == 3) {

            iFound = 1;
            iLanguage = ntohs(ao_pStAuxParams->m_NameRecord->m_LanguageID) & 0xff;
            iLength = ntohs(ao_pStAuxParams->m_NameRecord->m_StringLength);
            if (iLanguage == 0 || iLanguage == 9) {
                k = ntohs(ao_pStAuxParams->m_NameRecord->m_NameID);
                if (k < 8) {
                    ao_pStAuxParams->m_pcNameFields[k] = pcNameBuffer;

                    pcString = pcStringArea+ntohs(ao_pStAuxParams->m_NameRecord->m_StringOffset);
                    for ( k = 0; k < iLength; k++) {
                        if (pcString[k] != '\0') {
                            if (pcString[k] == '(') {
                                *pcNameBuffer = '[';
                            } else if (pcString[k] == ')') {
                                *pcNameBuffer = ']';
                            } else {
                                *pcNameBuffer = pcString[k];
                            };
                            pcNameBuffer ++;
						};
                    }
                    *pcNameBuffer = '\0';
                    pcNameBuffer ++;
                };
            };
        };
        ao_pStAuxParams->m_NameRecord++;
    };

    pcStringArea = (char *)ao_pStAuxParams->m_NameTable + ntohs(ao_pStAuxParams->m_NameTable->m_Offset);
    ao_pStAuxParams->m_NameRecord = &(ao_pStAuxParams->m_NameTable->m_NameRecords);

    if (!iFound) {
        for (j=0; j < ntohs(ao_pStAuxParams->m_NameTable->m_NumberOfNameRecords); j++) {

            iPlatform = ntohs(ao_pStAuxParams->m_NameRecord->m_PlatformID);

            if (iPlatform == 1) {

                iFound = 1;
                iLanguage = ntohs(ao_pStAuxParams->m_NameRecord->m_LanguageID) & 0xff;
                iLength = ntohs(ao_pStAuxParams->m_NameRecord->m_StringLength);
                if (iLanguage == 0 || iLanguage == 9) {
                    k = ntohs(ao_pStAuxParams->m_NameRecord->m_NameID);
                    if (k < 8) {
                        ao_pStAuxParams->m_pcNameFields[k] = pcNameBuffer;

                        pcString = pcStringArea+ntohs(ao_pStAuxParams->m_NameRecord->m_StringOffset);
                        for ( k = 0; k < iLength; k++) {
                            if (pcString[k] != '\0') {
                                if (pcString[k] == '(') {
                                    *pcNameBuffer = '[';
                                } else if (pcString[k] == ')') {
                                    *pcNameBuffer = ']';
                                } else {
                                    *pcNameBuffer = pcString[k];
                                };
                                pcNameBuffer ++;
                            };
                        };
                        *pcNameBuffer = '\0';
                        pcNameBuffer ++;
                    };
                };
            };
            ao_pStAuxParams->m_NameRecord++;
        };
    };

    if (!iFound) {
        fprintf(stderr, "**** Cannot decode font name fields ****\n");
        SwOk = M4_FALSE;
		goto exit;
    };

    if (ao_pStAuxParams->m_pcNameFields[6] == NULL) {
        ao_pStAuxParams->m_pcNameFields[6] = ao_pStAuxParams->m_pcNameFields[4];
    };

    pcString = ao_pStAuxParams->m_pcNameFields[6];
    while (*pcString != '\0') {
        if (!isalnum(*pcString)) {
            *pcString = '_';
        };
        pcString++;
    };

exit:

	return SwOk;
}

m4bool_t HandleCMap (StAuxParamsTTF* ao_pStAuxParams)
{
    m4int_t iNumTables = ntohs(ao_pStAuxParams->m_CMapTable->m_NumberOfEncodingTables);
	m4int_t iSize, iFormat, iOffset, iSegC2, iFound, iSetOk;
	m4int_t iPlatform, iEncodingID;
	TTF_CMAP_ENTRY *pTableEntry;
    TTF_CMAP_FMT0 *pEncoding0;
    TTF_CMAP_FMT4 *pEncoding4;
    TTF_WORD Start, End, Ro;
	m4bool_t SwOk = M4_TRUE;
    TTF_UCHAR *pcEncoder;
    m4int16_t Delta, Nu;
    m4int_t i, j, k, l;

    iFound = 0;

    for (i=0; i<256; i++) {
        ao_pStAuxParams->m_piEncoding[i] = 0;
    };

    for (i=0; i < iNumTables && !iFound; i++) {
        pTableEntry = &(ao_pStAuxParams->m_CMapTable->m_EncodingTable[i]);
        iOffset = ntohl(pTableEntry->m_Offset);
        pEncoding4 = (TTF_CMAP_FMT4 *) ((TTF_UCHAR *)ao_pStAuxParams->m_CMapTable + iOffset);
        iFormat = ntohs(pEncoding4->m_Format);
        iPlatform = ntohs(pTableEntry->m_PlatformID);
        iEncodingID = ntohs(pTableEntry->m_EncodingID);

        if (iPlatform == 3 && iFormat == 4) {
            switch (iEncodingID) {
                case 0: fputs("Found Symbol Encoding\n", stderr);
                        break;
                case 1: fputs("Found Unicode Encoding\n", stderr);
                        ao_pStAuxParams->m_iUnicode = 1;
                        break;
                default: fprintf(stderr,
                                 "****MS Encoding ID %d not supported****\n",
                                 iEncodingID);
                        SwOk = M4_FALSE;
						goto exit;
                        break;
            };

            iFound = 1;
            iSegC2 = ntohs(pEncoding4->m_SegCountX2);
            ao_pStAuxParams->m_iCMapNSegs = iSegC2 >> 1;
            pcEncoder = (TTF_UCHAR *)pEncoding4 + 14;
            ao_pStAuxParams->m_CMapSegEnd = (TTF_WORD *) pcEncoder;
            ao_pStAuxParams->m_CMapSegStart = (TTF_WORD *) (pcEncoder + iSegC2 + 2);
            ao_pStAuxParams->m_piCMapDeltaID = (short *) (pcEncoder + (iSegC2 * 2 )+ 2);
            ao_pStAuxParams->m_piCMapRangeOffsetID = (short *) (pcEncoder + (iSegC2 * 3) + 2);

            for (j=0; j < ao_pStAuxParams->m_iCMapNSegs-1; j++) {
                Start = ntohs(ao_pStAuxParams->m_CMapSegStart[j]);
                End   = ntohs(ao_pStAuxParams->m_CMapSegEnd[j]);
                Delta = ntohs(ao_pStAuxParams->m_piCMapDeltaID[j]);
                Ro    = ntohs(ao_pStAuxParams->m_piCMapRangeOffsetID[j]);

                for (k = Start; k <= End; k++)
                {
                    if (Delta != 0) {
                        Nu = k + Delta;
                    } else {
                        Nu = ntohs(*( (Ro >> 1) + (k - Start) +
                                    &(ao_pStAuxParams->m_piCMapRangeOffsetID[j])));
                    };

                    if (ao_pStAuxParams->m_pGlyphList[Nu].m_iUnicode != -1) {
                        if (strcmp(ao_pStAuxParams->m_pGlyphList[Nu].m_pcName, ".notdef") != 0) {
                            fprintf(stderr,
                                    "Glyph %s has >= two encodings (A), %4.4x & %4.4x\n",
                                    ao_pStAuxParams->m_pGlyphList[Nu].m_pcName,
                                    ao_pStAuxParams->m_pGlyphList[Nu].m_iUnicode,
                                    k);
                        };
                        iSetOk = 0;
                    } else {
                        iSetOk = 1;
                    };

                    if (ao_pStAuxParams->m_iUnicode) {
                        l = UnicodeToWin31 (k);
                        if (iSetOk) {
                            ao_pStAuxParams->m_pGlyphList[Nu].m_iUnicode = k;
                            ao_pStAuxParams->m_pGlyphList[Nu].m_iCharNo = l;
                        };
                        if (l <= 0xff) ao_pStAuxParams->m_piEncoding[l] = Nu;
                    } else {
                        if ((k & 0xff00) == 0xf000)
                        {
                            ao_pStAuxParams->m_piEncoding[k & 0x00ff] = Nu;
                            if (iSetOk) {
                                ao_pStAuxParams->m_pGlyphList[Nu].m_iCharNo = k & 0x00ff;
                                ao_pStAuxParams->m_pGlyphList[Nu].m_iUnicode = k;
                            };
                        } else {
                            if (iSetOk) {
                                ao_pStAuxParams->m_pGlyphList[Nu].m_iCharNo = k;
                                ao_pStAuxParams->m_pGlyphList[Nu].m_iUnicode = k;
                            };
                            fprintf(stderr,
                                    "Glyph %s has non-symbol encoding %4.4x\n",
                                    ao_pStAuxParams->m_pGlyphList[Nu].m_pcName,
                                    k & 0xffff);
                        };
                    };
                };
            };
        };
    };

    if (!iFound) {
        fputs ("No Microsoft encoding, looking for MAC encoding\n", stderr);
        for (i=0; i < iNumTables && !iFound; i++) {
            pTableEntry = &(ao_pStAuxParams->m_CMapTable->m_EncodingTable[i]);
            iOffset = ntohl(pTableEntry->m_Offset);
            pEncoding0 = (TTF_CMAP_FMT0 *) ((TTF_UCHAR *)ao_pStAuxParams->m_CMapTable + iOffset);
            iFormat = ntohs(pEncoding0->m_Format);
            iPlatform = ntohs(pTableEntry->m_PlatformID);
            iEncodingID = ntohs(pTableEntry->m_EncodingID);

            if (iFormat == 0) {
                iFound = 1;
                iSize = ntohs(pEncoding0->m_Length) - 6;
                for (j=0; j<iSize; j++) {
                    Nu = pEncoding0->m_GlyphIdArray[j];
                    if (ao_pStAuxParams->m_pGlyphList[Nu].m_iCharNo != -1) {
                        fprintf(stderr,
                                "Glyph %s has >= two encodings (B), %4.4x & %4.4x\n",
                                ao_pStAuxParams->m_pGlyphList[Nu].m_pcName,
                                ao_pStAuxParams->m_pGlyphList[Nu].m_iCharNo,
                                j);
                    } else {
                        ao_pStAuxParams->m_pGlyphList[Nu].m_iCharNo = j;
                        if (j < 256) {
                            ao_pStAuxParams->m_piEncoding[j] = Nu;
                        };
                    };
                };
            };
        };
    };

    if (!iFound) {
        fprintf(stderr, "**** No Recognised Encoding Table ****\n");
        SwOk=M4_FALSE;
		goto exit;
    };

exit:

	return SwOk;
}

m4bool_t HandleHead (StAuxParamsTTF* ao_pStAuxParams)
{
	m4bool_t SwOk=M4_TRUE;

    ao_pStAuxParams->m_iLongOffsets = ntohs(ao_pStAuxParams->m_HeadTable->m_IndexToLocFormat);
    
	if (ao_pStAuxParams->m_iLongOffsets != 0 && ao_pStAuxParams->m_iLongOffsets != 1) {
        fprintf(stderr, "**** m_IndexToLocFormat wrong ****\n");
        SwOk=M4_FALSE;
		goto exit;
    };

exit:

	return SwOk;
}

void DrawGlyf(m4int_t ai_iGlyphNo, m4int_t ai_iParent, StAuxParamsTTF* ao_pStAuxParams)
{
    m4int16_t xCoord[2000], yCoord[2000], xRel[2000], yRel[2000];
    m4int_t iFinished, iNGuide, iContourStart, iContourEnd;
    m4int16_t iNContours, iNInst, iLastPoint;
    m4int_t i, j, k, l, cs, ce;
    m4int_t iLength, iFirst;
    TTF_UCHAR Flags[2000];
    TTF_WORD *pContourEnd;
    TTF_UCHAR *pcChar;

    if (ao_pStAuxParams->m_iLongOffsets) {
        ao_pStAuxParams->m_GlyphTable = (TTF_GLYF *) (ao_pStAuxParams->m_GlyphStart + ntohl(ao_pStAuxParams->m_LongLocalTable[ai_iGlyphNo]));
        iLength = ntohl(ao_pStAuxParams->m_LongLocalTable[ai_iGlyphNo+1]) - ntohl(ao_pStAuxParams->m_LongLocalTable[ai_iGlyphNo]);
    } else {
        ao_pStAuxParams->m_GlyphTable = (TTF_GLYF *) (ao_pStAuxParams->m_GlyphStart + (ntohs(ao_pStAuxParams->m_ShortLocalTable[ai_iGlyphNo]) << 1));
        iLength = (ntohs(ao_pStAuxParams->m_ShortLocalTable[ai_iGlyphNo+1]) - ntohs(ao_pStAuxParams->m_ShortLocalTable[ai_iGlyphNo])) << 1;
    };

    if (iLength <= 0) {
        fprintf(stderr,
            "**** Composite glyph %s refers to non-existent glyph %s ****\n",
            ao_pStAuxParams->m_pGlyphList[ai_iParent].m_pcName,
            ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName);
        fprintf(ao_pStAuxParams->m_pFilePFA,
            "\n%%**** Composite glyph %s refers to non-existent glyph %s ****\n",
            ao_pStAuxParams->m_pGlyphList[ai_iParent].m_pcName,
            ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName);
        return;
    };

    iNContours = ntohs(ao_pStAuxParams->m_GlyphTable->m_NumberOfContours);
    if (iNContours <= 0) {
        fprintf(stderr,
                "**** Composite glyph %s refers to composite glyph %s ****\n",
                ao_pStAuxParams->m_pGlyphList[ai_iParent].m_pcName,
                ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName);
        fprintf(ao_pStAuxParams->m_pFilePFA,
                "\n%%**** Composite glyph %s refers to composite glyph %s ****\n",
                ao_pStAuxParams->m_pGlyphList[ai_iParent].m_pcName,
                ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName);
        return;
    };

    pContourEnd = (TTF_WORD *) ((char *)ao_pStAuxParams->m_GlyphTable + sizeof(TTF_GLYF));
    iLastPoint = ntohs(pContourEnd[iNContours-1]);
    iNInst = ntohs(pContourEnd[iNContours]);
    pcChar = ((TTF_UCHAR *)pContourEnd) + (iNContours << 1) + iNInst + 2;
    j = k = 0;

    while (k <= iLastPoint) {
        Flags[k] = pcChar[j];

        if (pcChar[j] & TTF_REPEAT) {
            for (l=0; l < pcChar[j+1]; l++) {
                k++;
                Flags[k] = pcChar[j];
            };
            j++;
        };
        j++; k++;
    };

    for (k=0; k <= iLastPoint; k++) {
        if (Flags[k] & TTF_XSHORT) {
            if (Flags[k] & TTF_XSAME) {
                xRel[k] = pcChar[j];
            } else {
                xRel[k] = - pcChar[j];
            };
            j++;
        } else if (Flags[k] & TTF_XSAME) {
            xRel[k] = 0;
        } else {
            xRel[k] = pcChar[j] * 256 + pcChar[j+1];
            j += 2;
        };
        if (k==0) {
            xCoord[k] = xRel[k];
        } else {
            xCoord[k] = xRel[k] + xCoord[k-1];
        };
    };

    for (k=0; k <= iLastPoint; k++) {
        if (Flags[k] & TTF_YSHORT) {
            if (Flags[k] & TTF_YSAME) {
                yRel[k] = pcChar[j];
            } else {
                yRel[k] = - pcChar[j];
            };
            j++;
        } else if (Flags[k] & TTF_YSAME) {
            yRel[k] = 0;
        } else {
            yRel[k] = pcChar[j] * 256 + pcChar[j+1];
            j += 2;
        };
        if (k==0) {
            yCoord[k] = yRel[k];
        } else {
            yCoord[k] = yRel[k] + yCoord[k-1];
        };
    };

    i = j = 0;
    iFirst = 1;

    while (i <= ntohs(pContourEnd[iNContours-1])) {
        iContourEnd = ntohs(pContourEnd[j]);

        if (iFirst) {
            fprintf(ao_pStAuxParams->m_pFilePFA, "%d %d moveto\n", xCoord[i], yCoord[i]);
            ao_pStAuxParams->m_iNCurves ++;
            iContourStart = i;
            iFirst = 0;
        } else if (Flags[i] & TTF_ONOFF) {
            fprintf(ao_pStAuxParams->m_pFilePFA, "%d %d lineto\n", xCoord[i], yCoord[i]);
            ao_pStAuxParams->m_iNCurves ++;
        } else {
            cs = i-1;
            iFinished = iNGuide = 0;
            while (!iFinished) {
                if (i == iContourEnd+1) {
                    ce = iContourStart;
                    iFinished = 1;
                } else if (Flags[i] & TTF_ONOFF) {
                    ce = i;
                    iFinished = 1;
                } else {
                    i++;
                    iNGuide++;
                };
            };

            switch (iNGuide) {
                case 0: fprintf( ao_pStAuxParams->m_pFilePFA,"%d %d lineto\n",
                                 xCoord[ce], yCoord[ce]);
                        ao_pStAuxParams->m_iNCurves ++;
                        break;

                case 1: fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (xCoord[cs]+2*xCoord[cs+1])/3,
                                (yCoord[cs]+2*yCoord[cs+1])/3,
                                (2*xCoord[cs+1]+xCoord[ce])/3,
                                (2*yCoord[cs+1]+yCoord[ce])/3,
                                xCoord[ce], yCoord[ce]);
                        ao_pStAuxParams->m_iNCurves ++;
                        break;

                case 2: fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (-xCoord[cs]+4*xCoord[cs+1])/3,
                                (-yCoord[cs]+4*yCoord[cs+1])/3,
                                (4*xCoord[cs+2]-xCoord[ce])/3,
                                (4*yCoord[cs+2]-yCoord[ce])/3,
                                xCoord[ce], yCoord[ce]);
                        ao_pStAuxParams->m_iNCurves ++;
                        break;

                case 3: fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (xCoord[cs]+2*xCoord[cs+1])/3,
                                (yCoord[cs]+2*yCoord[cs+1])/3,
                                (5*xCoord[cs+1]+xCoord[cs+2])/6,
                                (5*yCoord[cs+1]+yCoord[cs+2])/6,
                                (xCoord[cs+1]+xCoord[cs+2])/2,
                                (yCoord[cs+1]+yCoord[cs+2])/2);
                        fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (xCoord[cs+1]+5*xCoord[cs+2])/6,
                                (yCoord[cs+1]+5*yCoord[cs+2])/6,
                                (5*xCoord[cs+2]+xCoord[cs+3])/6,
                                (5*yCoord[cs+2]+yCoord[cs+3])/6,
                                (xCoord[cs+3]+xCoord[cs+2])/2,
                                (yCoord[cs+3]+yCoord[cs+2])/2);
                        fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (xCoord[cs+2]+5*xCoord[cs+3])/6,
                                (yCoord[cs+2]+5*yCoord[cs+3])/6,
                                (2*xCoord[cs+3]+xCoord[ce])/3,
                                (2*yCoord[cs+3]+yCoord[ce])/3,
                                xCoord[ce], yCoord[ce]);
                        ao_pStAuxParams->m_iNCurves += 3;
                        break;

                default:l = cs + iNGuide;

                        fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (xCoord[cs]+2*xCoord[cs+1])/3,
                                (yCoord[cs]+2*yCoord[cs+1])/3,
                                (5*xCoord[cs+1]+xCoord[cs+2])/6,
                                (5*yCoord[cs+1]+yCoord[cs+2])/6,
                                (xCoord[cs+1]+xCoord[cs+2])/2,
                                (yCoord[cs+1]+yCoord[cs+2])/2);
                        for (k = cs+2; k <= l-1; k++) {
                            fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                    (xCoord[k-1]+5*xCoord[k])/6,
                                    (yCoord[k-1]+5*yCoord[k])/6,
                                    (5*xCoord[k]+xCoord[k+1])/6,
                                    (5*yCoord[k]+yCoord[k+1])/6,
                                    (xCoord[k]+xCoord[k+1])/2,
                                    (yCoord[k]+yCoord[k+1])/2);
                        };

                        fprintf( ao_pStAuxParams->m_pFilePFA, "%d %d %d %d %d %d curveto\n",
                                (xCoord[l-1]+5*xCoord[l])/6,
                                (yCoord[l-1]+5*yCoord[l])/6,
                                (2*xCoord[l]+xCoord[ce])/3,
                                (2*yCoord[l]+yCoord[ce])/3,
                                xCoord[ce], yCoord[ce]);
                        ao_pStAuxParams->m_iNCurves += iNGuide;
                        break;
            };
        };

        if (i >= iContourEnd) {
            fprintf(ao_pStAuxParams->m_pFilePFA, " closepath ");
            iFirst = 1;
            i = iContourEnd + 1;
            j++;
        } else {
            i++;
        };
    };
}

m4float_t f2dot14 (m4int16_t ai_iX)
{
    m4int16_t y = ntohs(ai_iX);
    return (y >> 14) + ((y & 0x3fff) / 16384.0);
}

void DoGlyf(m4int_t ai_iGlyphNo, StAuxParamsTTF* ao_pStAuxParams)
{
    TTF_WORD iFlagByte, iGlyphIndex;
    m4int_t iLength, iCharNo;
    m4float_t fMatrix[4];
    m4int16_t iNContours;
    TTF_SHORT Arg1, Arg2;
    m4char_t *pcBuffer;
    TTF_UCHAR *pcChar;
    TTF_SHORT *pShort;

    ao_pStAuxParams->m_iNCurves = 0;

    if (ao_pStAuxParams->m_iLongOffsets) {
        ao_pStAuxParams->m_GlyphTable = (TTF_GLYF *) (ao_pStAuxParams->m_GlyphStart + ntohl(ao_pStAuxParams->m_LongLocalTable[ai_iGlyphNo]));
        iLength = ntohl(ao_pStAuxParams->m_LongLocalTable[ai_iGlyphNo+1]) - ntohl(ao_pStAuxParams->m_LongLocalTable[ai_iGlyphNo]);
    } else {
        ao_pStAuxParams->m_GlyphTable = (TTF_GLYF *) (ao_pStAuxParams->m_GlyphStart + (ntohs(ao_pStAuxParams->m_ShortLocalTable[ai_iGlyphNo]) << 1));
        iLength = (ntohs(ao_pStAuxParams->m_ShortLocalTable[ai_iGlyphNo+1]) - ntohs(ao_pStAuxParams->m_ShortLocalTable[ai_iGlyphNo])) << 1;
    };

    if (ao_pStAuxParams->m_iUnicode) {
        fprintf(ao_pStAuxParams->m_pFilePFA,
            "/%s { %% %d - U+%4.4x\n",
            ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName,
            ai_iGlyphNo,
            (unsigned short)ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_iUnicode);
    } else {
        fprintf(ao_pStAuxParams->m_pFilePFA,
            "/%s { %% %d - 0x%2.2x\n",
            ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName,
            ai_iGlyphNo,
            (unsigned short)ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_iCharNo);
    };

    iCharNo = ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_iCharNo;
    fprintf(ao_pStAuxParams->m_pFileAFM, "C %d ; WX %.0f ; N %s ; B %.0f %.0f %.0f %.0f ;\n",
        iCharNo < 256 ? iCharNo : -1,
        ao_pStAuxParams->m_fScaleFactor * ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_iWidth,
        ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName,
        ao_pStAuxParams->m_fScaleFactor * (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMinX),
        ao_pStAuxParams->m_fScaleFactor * (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMinY),
        ao_pStAuxParams->m_fScaleFactor * (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMaxX),
        ao_pStAuxParams->m_fScaleFactor * (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMaxY));

    fprintf(ao_pStAuxParams->m_pFilePFA, "%d 0 %hd %hd %hd %hd setcachedevice\n",
        ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_iWidth,
        (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMinX),
        (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMinY),
        (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMaxX),
        (short)ntohs(ao_pStAuxParams->m_GlyphTable->m_iMaxY));

    if (iLength != 0) {
        iNContours = ntohs(ao_pStAuxParams->m_GlyphTable->m_NumberOfContours);

        if (iNContours <= 0) {
            pcChar = ((TTF_UCHAR *)ao_pStAuxParams->m_GlyphTable + sizeof(TTF_GLYF));
            pShort = (TTF_SHORT *) pcChar;
            do {
                iFlagByte = ntohs(*pShort); pShort ++;
                iGlyphIndex = ntohs(*pShort); pShort ++;
                fprintf(ao_pStAuxParams->m_pFilePFA, "%% flags %x glyph %s\n",
                        iFlagByte, ao_pStAuxParams->m_pGlyphList[iGlyphIndex].m_pcName);

                if (iFlagByte & ARG_1_AND_2_ARE_WORDS) {
                    Arg1 = ntohs(*pShort); pShort++;
                    Arg2 = ntohs(*pShort); pShort++;
                } else {
                    pcBuffer = (char *)pShort;
                    Arg1 = (signed char)pcBuffer[0];
                    Arg2 = (signed char)pcBuffer[1];
                    pShort ++;
                };
                fMatrix[1] = fMatrix[2] = 0.0;

                if (iFlagByte & WE_HAVE_A_SCALE) {
                    fMatrix[0] = fMatrix[3] = f2dot14(*pShort);
                    pShort ++;
                } else if (iFlagByte & WE_HAVE_AN_X_AND_Y_SCALE) {
                    fMatrix[0] = f2dot14(*pShort); pShort ++;
                    fMatrix[3] = f2dot14(*pShort); pShort ++;
                } else if (iFlagByte & WE_HAVE_A_TWO_BY_TWO) {
                    fMatrix[0] = f2dot14(*pShort); pShort ++;
                    fMatrix[1] = f2dot14(*pShort); pShort ++;
                    fMatrix[2] = f2dot14(*pShort); pShort ++;
                    fMatrix[3] = f2dot14(*pShort); pShort ++;
                } else {
                    fMatrix[0] = fMatrix[3] = 1.0;
                };

                fprintf(ao_pStAuxParams->m_pFilePFA,
                        "matrix currentmatrix\n[ %9.7f %9.7f %9.7f %9.7f %hd %hd ] concat\n",
                        fMatrix[0], fMatrix[1], fMatrix[2], fMatrix[3],
                        Arg1, Arg2);

                DrawGlyf(iGlyphIndex, ai_iGlyphNo, ao_pStAuxParams);

                /*fputs("setmatrix closepath\n", m_pFilePFA);*/
                fputs("setmatrix\n", ao_pStAuxParams->m_pFilePFA);

            } while (iFlagByte & MORE_COMPONENTS);
        } else {
            DrawGlyf(ai_iGlyphNo, ai_iGlyphNo, ao_pStAuxParams);
            /*fprintf( m_pFilePFA, "closepath ");*/
        };

        if (ao_pStAuxParams->m_iNCurves > 100) {
            fprintf(stderr,
                    "**Glyf %s is too long, may have to be removed**\n",
                    ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName);
            fprintf(ao_pStAuxParams->m_pFilePFA,
                    "\n%%**Glyf %s is too long, may have to be removed**\n",
                    ao_pStAuxParams->m_pGlyphList[ai_iGlyphNo].m_pcName);
        };
    };

    fprintf(ao_pStAuxParams->m_pFilePFA, "fill } bind def\n");
}

void HandleHMTX(StAuxParamsTTF* ao_pStAuxParams)
{
    m4int_t iNHMetrics = ntohs(ao_pStAuxParams->m_HHeaTable->m_NumberOfHMetrics);
    LONGHORMETRIC *pHMTXEntry = ao_pStAuxParams->m_HMTXTable;
    TTF_WORD *pLSBList;
    Glyph *pGlyph;
    m4int_t i;

    for (i = 0; i < iNHMetrics; i++) {
        pGlyph = &(ao_pStAuxParams->m_pGlyphList[i]);
        pGlyph->m_iWidth = ntohs(pHMTXEntry->m_AdvanceWidth);
        pGlyph->m_iLSB = ntohs(pHMTXEntry->m_iLSB);
        pHMTXEntry++;
    };

    pLSBList = (TTF_WORD *)pHMTXEntry;
    pHMTXEntry--;

    for (i = iNHMetrics; i < ao_pStAuxParams->m_iNumGlyphs; i++) {
        pGlyph = &(ao_pStAuxParams->m_pGlyphList[i]);
        pGlyph->m_iWidth = ntohs(pHMTXEntry->m_AdvanceWidth);
        pGlyph->m_iLSB = ntohs(pLSBList[i-iNHMetrics]);
    };
}

m4bool_t HandlePost(StAuxParamsTTF* ao_pStAuxParams)
{
	m4char_t **ppcName = (m4char_t **) malloc (ao_pStAuxParams->m_iNumGlyphs * sizeof (m4char_t *));
    m4int_t *pNameLength = (m4int_t *) malloc (ao_pStAuxParams->m_iNumGlyphs * sizeof (m4int_t));
	m4int_t iLength, iFound;
	m4bool_t SwOk = M4_TRUE;
    TTF_WORD *pNameIndex;
	m4int_t iNPSNames;
    TTF_UINT iFormat;
    m4char_t *pcChar;
	m4int_t i, n;

    iFormat = ntohl(ao_pStAuxParams->m_PostTable->m_FormatType);

    if (iFormat == 0x00010000) {
        for (i=0; i<258; i++) {
            ao_pStAuxParams->m_pGlyphList[i].m_pcName = mac_glyph_names[i];
        };
    } else if (iFormat == 0x00020000) {
        n = ntohs(ao_pStAuxParams->m_PostTable->m_NumGlyphs);
        if (ao_pStAuxParams->m_iNumGlyphs != n) {
            fprintf(stderr, "**** Postscript table size mismatch %d/%d ****\n", n, ao_pStAuxParams->m_iNumGlyphs);
            SwOk=M4_FALSE;
			goto exit;
        };
        pcChar = (char *)ao_pStAuxParams->m_PostTable + 34 + (ao_pStAuxParams->m_iNumGlyphs << 1);
        iNPSNames = 0;
        while (*pcChar > 0) {
            iLength = pNameLength[iNPSNames] = *pcChar;
            ppcName[iNPSNames] = pcChar+1;
            *pcChar = '\0';
            iNPSNames ++;
            pcChar += iLength + 1;
        };
        *pcChar = '\0';

        pNameIndex = &(ao_pStAuxParams->m_PostTable->m_GlyphNameIndex);
        for (i=0; i<ao_pStAuxParams->m_iNumGlyphs; i++) {
            n = ntohs(pNameIndex[i]);
            if (n < 258) {
                ao_pStAuxParams->m_pGlyphList[i].m_pcName = mac_glyph_names[n];
            } else if (n < 258 + iNPSNames) {
                ao_pStAuxParams->m_pGlyphList[i].m_pcName = ppcName[n-258];
            } else {
                ao_pStAuxParams->m_pGlyphList[i].m_pcName = (char*) malloc(10);
                sprintf(ao_pStAuxParams->m_pGlyphList[i].m_pcName, "_%d", n);
                fprintf(stderr,
                        "**** Glyph No. %d has no postscript name, becomes %s ****\n",
                        i, ao_pStAuxParams->m_pGlyphList[n].m_pcName);
            };
        };
    } else if (iFormat == 0x00030000) {
        fputs ("No postscript table, using default\n", stderr);
        ao_pStAuxParams->m_iPsFmt3 = 1;
    } else if (iFormat == 0x00028000) {
        pcChar = (char *)&(ao_pStAuxParams->m_PostTable->m_NumGlyphs);
        for (i=0; i<ao_pStAuxParams->m_iNumGlyphs; i++) {
            ao_pStAuxParams->m_pGlyphList[i].m_pcName = mac_glyph_names[i + pcChar[i]];
        };
    } else {
        fprintf(stderr,
                "**** Postscript table in wrong format %x ****\n",
                iFormat);
        SwOk=M4_FALSE;
		goto exit;
    };

    if (!ao_pStAuxParams->m_iPsFmt3) {
        for (n = 0; n < ao_pStAuxParams->m_iNumGlyphs; n++) {
            iFound = 0;
            for (i = 0; i < n && !iFound; i++) {
                if (strcmp(ao_pStAuxParams->m_pGlyphList[i].m_pcName, ao_pStAuxParams->m_pGlyphList[n].m_pcName) == 0) {
                    ao_pStAuxParams->m_pGlyphList[n].m_pcName = (char*) malloc(10);
                    sprintf(ao_pStAuxParams->m_pGlyphList[n].m_pcName, "_%d", n);
                    fprintf(stderr,
                            "Glyph %d has the same name as %d: (%s), changing to %s\n",
                            n, i,
                            ao_pStAuxParams->m_pGlyphList[i].m_pcName,
                            ao_pStAuxParams->m_pGlyphList[n].m_pcName);
                    iFound = 1;
                };
            };
        };
    };

exit: 

	return SwOk;
}

void HandleKern(StAuxParamsTTF* ao_pStAuxParams)
{
    m4int_t iNTables = ntohs(ao_pStAuxParams->m_KernTable->m_nTables);
    m4char_t *pcChar = (m4char_t *)ao_pStAuxParams->m_KernTable + 4;
	TTF_KERN_ENTRY *pKernEntry;
    TTF_KERN_SUB *pSubTable;
	m4int_t iNPairs;
    m4int_t i, j;

    for (i=0; i < iNTables; i++) {
        pSubTable = (TTF_KERN_SUB *)pcChar;
        if ((ntohs(pSubTable->m_Coverage) & 0xff00) == 0) {
            iNPairs = (short)ntohs(pSubTable->m_nPairs);
            fprintf(ao_pStAuxParams->m_pFileAFM, "StartKernPairs %hd\n", iNPairs);
            pKernEntry = (TTF_KERN_ENTRY *)(pcChar + sizeof(TTF_KERN_SUB));
            for (j=0; j<iNPairs; j++) {
                fprintf(ao_pStAuxParams->m_pFileAFM, "KPX %s %s %.0f\n",
                        ao_pStAuxParams->m_pGlyphList[ntohs(pKernEntry->m_Left)].m_pcName,
                        ao_pStAuxParams->m_pGlyphList[ntohs(pKernEntry->m_Right)].m_pcName,
                        ao_pStAuxParams->m_fScaleFactor * (short) ntohs(pKernEntry->m_Value));
                pKernEntry ++;
            };
            fprintf(ao_pStAuxParams->m_pFileAFM, "EndKernPairs\n");
        };
        pcChar += pSubTable->m_Length;
    };
}

m4bool_t ClRFN_RFN_W::ConvertFromTTF_To_PFA(m4char_t *ai_pcTTFFile, m4char_t *ai_pcPFAFile)
{
	StMIT_Inspector * Insp = GET_INSP() ;
	StAuxParamsTTF AuxParamsTTF;
    m4char_t pcFileName[100];
	m4float_t fItalicAngle;
    struct stat StatBuf;
	m4bool_t SwOk;
    time_t tNow;
	m4int_t i;
	eUniFileEncodingType_t eEncoding = M4UniANSI ;

	SwOk = M4_TRUE;

	//Inicializamos parametros auxiliares
	AuxParamsTTF.m_NameTable = NULL;
	AuxParamsTTF.m_HeadTable = NULL;
	AuxParamsTTF.m_HHeaTable = NULL;
	AuxParamsTTF.m_KernTable = NULL;
	AuxParamsTTF.m_CMapTable = NULL;
	AuxParamsTTF.m_HMTXTable = NULL;
	AuxParamsTTF.m_GlyphStart = NULL;
	AuxParamsTTF.m_MaxPTable = NULL;
	AuxParamsTTF.m_PostTable = NULL;
	AuxParamsTTF.m_ShortLocalTable = NULL;
	AuxParamsTTF.m_LongLocalTable = NULL;
	AuxParamsTTF.m_iPsFmt3 = 0;
	AuxParamsTTF.m_iUnicode = 0;
	strcpy(AuxParamsTTF.m_pcUnknownGlyph, "UNKN");

	// UNICODE FILE
	int iLength = -1 ;
	char* pcFile = M4CppToANSI( ai_pcTTFFile, iLength ) ;
	int ret = stat( pcFile, &StatBuf ) ;
	delete [] pcFile ;

    if (ret == -1) {
        fprintf(stderr, "**** Cannot access %s ****\n", ai_pcTTFFile);
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unable to access TTF File: %0:s" ) 
			<< ai_pcTTFFile << SEND_MSG ;
		SwOk=M4_FALSE;
        goto exit;
    };

    if ((AuxParamsTTF.m_pcFileBuffer = (char*) malloc (StatBuf.st_size)) == NULL) {
        fprintf(stderr, "**** Cannot malloc space for file ****\n");
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unable to alocate memory for file" ) << SEND_MSG ;
        SwOk=M4_FALSE;
        goto exit;
    };

	// UNICODE FILE
	iLength = -1 ;
	pcFile = M4CppToANSI( ai_pcTTFFile, iLength ) ;

	AuxParamsTTF.m_iFileTTF = open( pcFile, O_RDONLY | O_BINARY, 0 ) ;
	delete [] pcFile ;

    if (AuxParamsTTF.m_iFileTTF == -1) {
        fprintf(stderr, "**** Cannot open %s ****\n", ai_pcTTFFile);
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unable to open TTF File: %0:s" ) 
			<< ai_pcTTFFile << SEND_MSG ;
        SwOk=M4_FALSE;
        goto exit;
    } else {
        fprintf(stderr, "Processing file %s\n", ai_pcTTFFile);
    };

    if (read(AuxParamsTTF.m_iFileTTF, AuxParamsTTF.m_pcFileBuffer, StatBuf.st_size) != StatBuf.st_size) {
        fprintf(stderr, "**** Could not read whole file ****\n");
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unable to read whole TTF File" ) << SEND_MSG ;
        SwOk=M4_FALSE;
        goto exit;
    };

    AuxParamsTTF.m_DirTTF = (TTF_DIRECTORY *) AuxParamsTTF.m_pcFileBuffer;

    if (ntohl(AuxParamsTTF.m_DirTTF->m_SFntVersion) != 0x00010000) {
        fprintf(stderr,
                "****Unknown File Version number [%x], or not a TrueType file****\n",
                AuxParamsTTF.m_DirTTF->m_SFntVersion);
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unknown file version number, or not a True Type File" ) 
			<< SEND_MSG ;
        SwOk=M4_FALSE;
        goto exit;
    };

    sprintf(pcFileName, "%s.pfa", ai_pcPFAFile) ;

	// UNICODE FILE
	eEncoding = M4UniANSI ;

    if ((AuxParamsTTF.m_pFilePFA = M4Fopen( pcFileName, M4UniWrite, eEncoding )) == NULL)
	{
        fprintf(stderr, "**** Cannot create %s ****\n", pcFileName);
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unable to create PFA File: %0:s" ) 
			<< pcFileName << SEND_MSG ;
        SwOk=M4_FALSE;
        goto exit;
    } else {
        fprintf(stderr, "Creating file %s\n", pcFileName);
    };

    sprintf(pcFileName, "%s.afm", ai_pcPFAFile) ;

	// UNICODE FILE
	eEncoding = M4UniANSI ;

    if ((AuxParamsTTF.m_pFileAFM = M4Fopen( pcFileName, M4UniWrite, eEncoding )) == NULL)
	{
        fprintf(stderr, "**** Cannot create %s ****\n", pcFileName);
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Unable to create AFM File: %0:s" ) 
			<< pcFileName << SEND_MSG ;
        SwOk=M4_FALSE;
        goto exit;
    };

    AuxParamsTTF.m_DirEntry = &(AuxParamsTTF.m_DirTTF->m_List);

    for (i=0; i < ntohs(AuxParamsTTF.m_DirTTF->m_NumTables); i++) {

        if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "name", 4) == 0) {
            AuxParamsTTF.m_NameTable = (TTF_NAME *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "head", 4) == 0) {
            AuxParamsTTF.m_HeadTable = (TTF_HEAD *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "hhea", 4) == 0) {
            AuxParamsTTF.m_HHeaTable = (TTF_HHEA *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "post", 4) == 0) {
            AuxParamsTTF.m_PostTable = (TTF_POST_HEAD *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "glyf", 4) == 0) {
            AuxParamsTTF.m_GlyphStart = (TTF_UCHAR *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "cmap", 4) == 0) {
            AuxParamsTTF.m_CMapTable = (TTF_CMAP *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "kern", 4) == 0) {
            AuxParamsTTF.m_KernTable = (TTF_KERN *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "maxp", 4) == 0) {
            AuxParamsTTF.m_MaxPTable = (TTF_MAXP *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "hmtx", 4) == 0) {
            AuxParamsTTF.m_HMTXTable = (LONGHORMETRIC *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "loca", 4) == 0) {
            AuxParamsTTF.m_LongLocalTable = (TTF_UINT *) (AuxParamsTTF.m_pcFileBuffer+ntohl(AuxParamsTTF.m_DirEntry->m_Offset));
            AuxParamsTTF.m_ShortLocalTable = (TTF_WORD *) AuxParamsTTF.m_LongLocalTable;
        } else if (memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "EBDT", 4) == 0 ||
                   memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "EBLC", 4) == 0 ||
                   memcmp(AuxParamsTTF.m_DirEntry->m_cTag, "EBSC", 4) == 0) {
            fprintf (stderr, "Font contains bitmaps\n");
        }
        AuxParamsTTF.m_DirEntry++;
    };

    if(!HandleName(&AuxParamsTTF)) { 
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Error in HandleName conversion section" ) << SEND_MSG ;
		SwOk = M4_FALSE;
		goto exit;
	};

    if(!HandleHead(&AuxParamsTTF)){
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Error in HandleHead conversion section" ) << SEND_MSG ;
		SwOk = M4_FALSE;
		goto exit;
	};

    AuxParamsTTF.m_iNumGlyphs = ntohs(AuxParamsTTF.m_MaxPTable->m_NumGlyphs);
    fprintf(stderr, "Glyphs Number = %d\n", AuxParamsTTF.m_iNumGlyphs);
    AuxParamsTTF.m_pGlyphList = (Glyph *) malloc (AuxParamsTTF.m_iNumGlyphs * sizeof (Glyph));

    for (i=0; i<AuxParamsTTF.m_iNumGlyphs; i++) {
        AuxParamsTTF.m_pGlyphList[i].m_iCharNo = -1;
        AuxParamsTTF.m_pGlyphList[i].m_iUnicode = -1;
        AuxParamsTTF.m_pGlyphList[i].m_pcName = AuxParamsTTF.m_pcUnknownGlyph;
    };

    HandlePost(&AuxParamsTTF);
    HandleHMTX(&AuxParamsTTF);
    
	if(!HandleCMap(&AuxParamsTTF)){
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Error in HandleCMap conversion section" ) << SEND_MSG ;
		SwOk=M4_FALSE;
		goto exit;
	};

    if (AuxParamsTTF.m_iPsFmt3) {
        for (i=0; i<255; i++) {
            if (AuxParamsTTF.m_piEncoding[i] != 0) {
                AuxParamsTTF.m_pGlyphList[AuxParamsTTF.m_piEncoding[i]].m_pcName = ISOLatin1Encoding[i];
            } else {
                AuxParamsTTF.m_pGlyphList[AuxParamsTTF.m_piEncoding[i]].m_pcName = ".notdef";
            };
        };
    };

    fprintf(AuxParamsTTF.m_pFilePFA, "%%!PS-Adobe-3.0 Resource-Font\n");
    time(&tNow);
    fprintf(AuxParamsTTF.m_pFilePFA, "%%%%CreationDate: %s", ctime(&tNow));
    fprintf(AuxParamsTTF.m_pFilePFA, "%%%%BeginFont: %s\n", AuxParamsTTF.m_pcNameFields[6]);
    fprintf(AuxParamsTTF.m_pFilePFA, "%% Converted from TrueType font %s by RFNBuilder\n%%\n", ai_pcTTFFile);
    fprintf(AuxParamsTTF.m_pFileAFM, "StartFontMetrics 4.1\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "20 dict begin\n/FontType 3 def\n");
    fprintf(AuxParamsTTF.m_pFileAFM, "FontName %s\n", AuxParamsTTF.m_pcNameFields[6]);
    fprintf(stderr, "FontName %s\n", AuxParamsTTF.m_pcNameFields[6]);
    fprintf(AuxParamsTTF.m_pFilePFA, "/FontName (%s) def\n", AuxParamsTTF.m_pcNameFields[6]);
    fprintf(AuxParamsTTF.m_pFilePFA, "/FontInfo 9 dict dup begin\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "/FullName (%s) def\n", AuxParamsTTF.m_pcNameFields[4]);
    fprintf(AuxParamsTTF.m_pFileAFM, "FullName %s\n", AuxParamsTTF.m_pcNameFields[4]);
    fprintf(AuxParamsTTF.m_pFilePFA, "/Notice (%s) def\n", AuxParamsTTF.m_pcNameFields[0]);
    fprintf(AuxParamsTTF.m_pFileAFM, "Notice %s\n", AuxParamsTTF.m_pcNameFields[0]);
    fprintf(AuxParamsTTF.m_pFilePFA, "/FamilyName (%s) def\n", AuxParamsTTF.m_pcNameFields[1]);
    fprintf(AuxParamsTTF.m_pFileAFM, "FamilyName %s\n", AuxParamsTTF.m_pcNameFields[1]);
    fprintf(AuxParamsTTF.m_pFilePFA, "/Weight (%s) def\n", AuxParamsTTF.m_pcNameFields[2]);
    fprintf(AuxParamsTTF.m_pFileAFM, "Weight %s\n", AuxParamsTTF.m_pcNameFields[2]);
    fprintf(AuxParamsTTF.m_pFilePFA, "/version (%s) def\n", AuxParamsTTF.m_pcNameFields[5]);
    fprintf(AuxParamsTTF.m_pFileAFM, "Version %s\n", AuxParamsTTF.m_pcNameFields[5]);
    fprintf(AuxParamsTTF.m_pFileAFM, "Characters %d\n", AuxParamsTTF.m_iNumGlyphs);
    fItalicAngle = (short)(ntohs(AuxParamsTTF.m_PostTable->m_ItalicAngle.m_Upper)) +
                    (ntohs(AuxParamsTTF.m_PostTable->m_ItalicAngle.m_Lower) / 65536.0);
    fprintf(AuxParamsTTF.m_pFilePFA, "/italicAngle %f def\n", fItalicAngle);
    fprintf(AuxParamsTTF.m_pFileAFM, "ItalicAngle %f\n", fItalicAngle);
    AuxParamsTTF.m_fScaleFactor = 1000.0 / ntohs(AuxParamsTTF.m_HeadTable->m_UnitsPerEm);
    fprintf(AuxParamsTTF.m_pFileAFM, "Ascender %.0f\n",
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_HHeaTable->m_Ascender));
    fprintf(AuxParamsTTF.m_pFileAFM, "Descender %.0f\n",
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_HHeaTable->m_Descender));
    fprintf(AuxParamsTTF.m_pFilePFA, "/underlineThickness %hd def\n",
            (short)ntohs(AuxParamsTTF.m_PostTable->m_UnderlineThickness));
    fprintf(AuxParamsTTF.m_pFileAFM, "UnderlineThickness %.0f\n",
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_PostTable->m_UnderlineThickness));
    fprintf(AuxParamsTTF.m_pFilePFA, "/underlinePosition %hd def\n",
            (short)ntohs(AuxParamsTTF.m_PostTable->m_UnderlinePosition));
    fprintf(AuxParamsTTF.m_pFileAFM, "UnderlinePosition %.0f\n",
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_PostTable->m_UnderlinePosition));
    fprintf(AuxParamsTTF.m_pFilePFA, "/isFixedPitch %s def end def\n",
            ntohl(AuxParamsTTF.m_PostTable->m_IsFixedPitch) ? "true" : "false" );
    fprintf(AuxParamsTTF.m_pFileAFM, "IsFixedPitch %s\n",
            ntohl(AuxParamsTTF.m_PostTable->m_IsFixedPitch) ? "true" : "false" );
    fprintf(AuxParamsTTF.m_pFilePFA, "/FontMatrix [%9.7f 0 0 %9.7f 0 0] def\n",
            AuxParamsTTF.m_fScaleFactor/1000.0, AuxParamsTTF.m_fScaleFactor/1000.0);
    fprintf(AuxParamsTTF.m_pFilePFA, "/FontBBox [%hd %hd %hd %hd] def\n",
            (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMinX),
            (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMinY),
            (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMaxX),
            (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMaxY));
    fprintf(AuxParamsTTF.m_pFileAFM, "FontBBox %.0f %.0f %.0f %.0f\n",
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMinX),
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMinY),
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMaxX),
            AuxParamsTTF.m_fScaleFactor * (short)ntohs(AuxParamsTTF.m_HeadTable->m_iMaxY));
    fprintf(AuxParamsTTF.m_pFilePFA, "/Encoding [\n");

    for (i=0; i<256; i++) {
        fprintf(AuxParamsTTF.m_pFilePFA,
                "/%s ",
                AuxParamsTTF.m_pGlyphList[AuxParamsTTF.m_piEncoding[i]].m_pcName);
        if (i%4 == 3) {
            fprintf(AuxParamsTTF.m_pFilePFA, "%% 0x%x\n", i-3);
        };
    };

    fprintf(AuxParamsTTF.m_pFilePFA,
            "] def\n/CharProcs %d dict def CharProcs begin\n",
            AuxParamsTTF.m_iNumGlyphs+1);
    fprintf(AuxParamsTTF.m_pFileAFM, "StartCharMetrics %d\n", AuxParamsTTF.m_iNumGlyphs);

    for (i=0; i<AuxParamsTTF.m_iNumGlyphs; i++) {
        DoGlyf(i, &AuxParamsTTF);
    };

    fprintf(AuxParamsTTF.m_pFileAFM, "EndCharMetrics\n");

    if (AuxParamsTTF.m_KernTable != NULL) {
        fprintf(AuxParamsTTF.m_pFileAFM, "StartKernData\n");
        HandleKern(&AuxParamsTTF);
        fprintf(AuxParamsTTF.m_pFileAFM, "EndKernData\n");
    } else {
        fputs("No Kerning data\n", stderr);
    };

    fprintf(AuxParamsTTF.m_pFilePFA, "end\n/BuildGlyph {\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "exch /CharProcs get exch\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "2 copy known not {pop /.notdef} if ");
    fprintf(AuxParamsTTF.m_pFilePFA, "get exec } bind def\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "/BuildChar { 1 index /Encoding get exch get\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "1 index /Encoding get exec } bind def\n");
    fprintf(AuxParamsTTF.m_pFilePFA,
            "currentdict end /%s exch definefont pop\n",
            AuxParamsTTF.m_pcNameFields[6]);
    fprintf(AuxParamsTTF.m_pFileAFM, "EndFontMetrics\n");
    fprintf(AuxParamsTTF.m_pFilePFA, "%%%%EndFont\n");

    fclose(AuxParamsTTF.m_pFileAFM);
    fclose(AuxParamsTTF.m_pFilePFA);

    fprintf(stderr, "Finished - font files created\n");

exit:

	return SwOk;
}
/* ArrowEffects2 - the squeakuel */

#ifndef ARROWEFFECTS2_H
#define ARROWEFFECTS2_H
#include "PlayerOptions.h"
#include "Style.h"

struct PlayerState;



class ArrowEffects2
{
public:
    //NoteDisplay needs to be able to hold onto these objects, but it doesn't
    //need to be able to read or write them.

    //Holds state that will remain constant for a NoteDisplay's lifetime.
    class SongState
    {
    private:
        friend class ArrowEffects2;

        int iColumns;
        float fZoom;
        bool bEditing;
        float fBaseXOffsets[MAX_COLS_PER_PLAYER];
        float fTornadoXOffsets[MAX_COLS_PER_PLAYER];
    };

    //Holds state that will remain constant for one frame.
    class FrameState
    {
    private:
        friend class ArrowEffects2;

        //Mods, but also parameters that depend on them.
        //Mods that don't show up in ArrowEffects don't appear at all. If all
        //or some of the effects of a mod can be calculated per frame, that is
        //done here also.
	    struct Mods
        {
            //CMX mods
            float fTimeSpacing;
            float fScrollSpeed;
            float fRandomSpeed;
            float fScrollBPS;

            //reverse and centered
            float fReverseShifts[MAX_COLS_PER_PLAYER];
            float fReverseScales[MAX_COLS_PER_PLAYER];

            float fFlip;
            float fInvert;
            
            //"shaky mods"
            float fBeatOffset; //beat is frame invariant so the whole thing is calulated here
            float fTipsyOffsets[MAX_COLS_PER_PLAYER];
            float fTornado;
            float fDrunk;

            //accels
            float fBoost;
            float fBrake;
            float fWave;
            float fBoomerang;

            float fRoll;
            float fTwirl;
            float fDizzy;

            float fBumpy;

            float fHidden;
            float fHiddenOffset;
            float fSudden;
            float fSuddenOffset;
            float fStealth;
            float fRandomVanish;

            float fMini;
            float fCenterLine;

        };
        Mods m_Mods;
        float fNoteFieldHeight;
        float fSongBeat;
        float fSongSeconds;
        float fBPS;
        bool bNeedZBuffer;
        bool bFreeze;
        bool bEditing;
        float fPercentFadeToFail;
    public:
        bool NeedZBuffer() const { return bNeedZBuffer; }
    };

    //Enough information about a note to calculate its PointProperties given
    //the SongState and FrameState
    struct NoteDescription
    {
	    int iCol;
	    float fNoteBeat;
    };

    //Enough information to render a point (note or hold body portion)
    struct PointProperties
    {
        int iCol;
	    float fGlow;
	    float fAlpha;
	    float fBrightness;
	    float fPositionX;
	    float fPositionY;
	    float fPositionZ;
	    float fRotationX;
	    float fRotationY;
	    float fRotationZ;
	    float fOffsetY;
    };
    static const FrameState GetFrameState(const SongState &pSongState, const PlayerState &pPlayerState);
private:
    //Literally only used as a way to pack the hidden and sudden lines
    struct HiddenSuddenLines
    {
	    float fHiddenStartLine;
	    float fHiddenEndLine;
	    float fSuddenStartLine;
        float fSuddenEndLine;
    };

    static const HiddenSuddenLines ArrowEffects2::GetHiddenSuddenLines(const FrameState &state);

	//	fYOffset is a vertical position in pixels relative to the center.
	//	(positive if has not yet been stepped on, negative if has already passed).
	//	The ArrowEffect and ScrollSpeed is applied in this stage.
	static float GetYOffset( const PlayerState* pPlayerState, int iCol, float fNoteBeat, float &fPeakYOffsetOut, bool &bIsPastPeakYOffset, bool bAbsolute=false );
	static float GetYOffset( const PlayerState* pPlayerState, int iCol, float fNoteBeat, bool bAbsolute=false )
	{
		float fThrowAway;
		bool bThrowAway;
		return GetYOffset( pPlayerState, iCol, fNoteBeat, fThrowAway, bThrowAway, bAbsolute );
	}

    static float ArrowEffects2::ArrowGetPercentVisible( const PlayerState* pPlayerState, int iCol, float fYOffset, float fYReverseOffsetPixels );

	/* Actual display position, with reverse and post-reverse-effects factored in
	 * (fYOffset -> YPos). */
	static float GetYPos(	const PlayerState* pPlayerState, int iCol, float fYOffset, float fYReverseOffsetPixels, bool WithReverse = true );

	// Inverse of ArrowGetYPos (YPos -> fYOffset).
	static float GetYOffsetFromYPos( const PlayerState* pPlayerState, int iCol, float YPos, float fYReverseOffsetPixels );


	//	fRotation is Z rotation of an arrow.  This will depend on the column of 
	//	the arrow and possibly the Arrow effect and the fYOffset (in the case of 
	//	EFFECT_DIZZY).
	static float GetRotationZ(	const FrameState &state, const float fBeatOffset );

	// due to the handling logic for holds on Twirl, we need to use an offset instead.
	// it's more intuitive for Roll to be based off offset, so use an offset there too.
	static float GetRotationX(  const FrameState &state, const float fBeatOffset );
	static float GetRotationY(  const FrameState &state, const float fBeatOffset );

	//	fXPos is a horizontal position in pixels relative to the center of the field.
	//	This depends on the column of the arrow and possibly the Arrow effect and
	//	fYPos (in the case of EFFECT_DRUNK).
	static float GetXPos( const PlayerState* pPlayerState, int iCol, float fYOffset );

	//  Z position; normally 0.  Only visible in perspective modes.
	static float GetZPos( const FrameState &state, const float fYOffset );

	// Enable this if any ZPos effects are enabled.
	static bool NeedZBuffer( const FrameState &state );

	//	fAlpha is the transparency of the arrow.  It depends on fYPos and the 
	//	AppearanceType.
	static float GetAlpha( const PlayerState* pPlayerState, int iCol, float fYPos, float fPercentFadeToFail, float fYReverseOffsetPixels );


	//	fAlpha is the transparency of the arrow.  It depends on fYPos and the 
	//	AppearanceType.
	static float GetGlow( const PlayerState* pPlayerState, int iCol, float fYPos, float fPercentFadeToFail, float fYReverseOffsetPixels );


	//	Depends on fYOffset.
	static const float GetBrightness( const FrameState &state, const float fBeatDistance );

	// This is the zoom of the individual tracks, not of the whole Player.
	static float GetZoom( const PlayerState* pPlayerState );

    static const float ArrowEffects2::GetCenterLine( const FrameState &state );
    static const float ArrowEffects2::GetHiddenSudden( const FrameState &state );


};

#endif

/*
 * (c) 2001-2004 Chris Danford
 * (c) 2022 tertu marybig
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

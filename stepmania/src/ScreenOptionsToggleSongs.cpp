#include "global.h"
#include "ScreenOptionsToggleSongs.h"
#include "OptionRowHandler.h"
#include "RageUtil.h"
#include "ScreenManager.h"
#include "song.h"
#include "SongManager.h"
#include "UnlockManager.h"

REGISTER_SCREEN_CLASS( ScreenOptionsToggleSongs );

void ScreenOptionsToggleSongs::BeginScreen()
{
	m_apSongs.clear();

	vector<OptionRowHandler*> vHands;

	const vector<Song *> &apAllSongs = SONGMAN->GetSongs();
	FOREACH_CONST( Song *, apAllSongs , s )
	{
		Song *pSong = *s;
		if( UNLOCKMAN->SongIsLocked(pSong) & ~LOCKED_DISABLED )
			continue;

		vHands.push_back( OptionRowHandlerUtil::MakeNull() );
		OptionRowDefinition &def = vHands.back()->m_Def;

		def.m_sName = pSong->GetTranslitFullTitle();
		def.m_bAllowThemeTitle = false;	// not themable
		def.m_sExplanationName = "Toggle Song";
		def.m_vsChoices.clear();
		def.m_vsChoices.push_back( "On" );
		def.m_vsChoices.push_back( "Off" );
		def.m_bAllowThemeItems = false;	// already themed

		m_apSongs.push_back( pSong );
	}

	InitMenu( vHands );

	ScreenOptions::BeginScreen();
}

void ScreenOptionsToggleSongs::ImportOptions( int iRow, const vector<PlayerNumber> &vpns )
{
	for( int i = 0; i < (int) m_apSongs.size(); ++i )
	{
		OptionRow &row = *m_pRows[i];
		bool bEnable = m_apSongs[i]->GetEnabled();
		int iSelection = bEnable? 0:1;
		row.SetOneSharedSelection( iSelection );
	}
}

void ScreenOptionsToggleSongs::ExportOptions( int iRow, const vector<PlayerNumber> &vpns )
{
	for( int i = 0; i < (int) m_apSongs.size(); ++i )
	{
		const OptionRow &row = *m_pRows[i];
		int iSelection = row.GetOneSharedSelection();
		bool bEnable = (iSelection == 0);
		m_apSongs[i]->SetEnabled( bEnable );
	}

	SONGMAN->SaveEnabledSongsToPref();
}

/*
 * (c) 2007 Glenn Maynard
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
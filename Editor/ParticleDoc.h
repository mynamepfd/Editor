#pragma once

#include "ScintillaDocView.h"

class ParticleDoc : public Scintilla::CScintillaDoc
{
	DECLARE_DYNCREATE(ParticleDoc)
public:
	ParticleDoc();
	virtual ~ParticleDoc();

	DECLARE_MESSAGE_MAP()
};

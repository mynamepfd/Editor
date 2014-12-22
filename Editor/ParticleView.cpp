#include "stdafx.h"
#include "Editor.h"
#include "ParticleDoc.h"
#include "ParticleView.h"
#include "ParticlePreviewPage.h"
#include "StringUtils.h"

#include "OgreString.h"

const char gParticle0[] = 
	"";
const char gParticle1[] = 
	"";

IMPLEMENT_DYNCREATE(ParticleView, Scintilla::CScintillaView)

ParticleView::ParticleView()
{
}

ParticleView::~ParticleView()
{
}

BEGIN_MESSAGE_MAP(ParticleView, Scintilla::CScintillaView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void ParticleView::OnDestroy()
{
	CScintillaView::OnDestroy();

	std::string extension = StringUtils::extension(std::string(GetDocument()->GetPathName()));
	if(extension == "particle")
	{
		ParticlePreviewPage::current->clearParticleList();
	}
}

void ParticleView::OnSetFocus(CWnd* pOldWnd)
{
	CScintillaView::OnSetFocus(pOldWnd);
	parseFile();
}

void ParticleView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		parseFile();
	}
}

void ParticleView::OnInitialUpdate()
{
	CScintillaView::OnInitialUpdate();

	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();

	rCtrl.SetLexer(SCLEX_CPP);

	std::string extension = StringUtils::extension(std::string(GetDocument()->GetPathName()));
	if(extension == "particle")
	{
		rCtrl.SetKeyWords(0, gParticle0);
		rCtrl.SetKeyWords(1, gParticle1);
	}

	setAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 13, "Consolas"); // 字体
	rCtrl.SetMarginTypeN(0, SC_MARGIN_NUMBER); // 行号
	rCtrl.SetMarginWidthN(0, 48);

	rCtrl.StyleClearAll();
	setAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
	setAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
	setAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
	setAStyle(SCE_C_WORD2, RGB(0x2B, 0x91, 0xAF));
	setAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
	setAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
	setAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
	setAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

	rCtrl.SetCaretLineVisible(TRUE); // 当前行高亮
	rCtrl.SetCaretLineBack(RGB(0xff, 0xff, 0xb0));

	rCtrl.SetTabWidth(4);

	parseFile();
}

void ParticleView::OnCharAdded(Scintilla::SCNotification* pSCNotification)
{
	lastChar = pSCNotification->ch;

	CDocument *document = GetDocument();
	document->SetModifiedFlag();
}

void ParticleView::OnUpdateUI(Scintilla::SCNotification* pSCNotification)
{
	KillTimer(1);
	SetTimer(1, 30, NULL);

	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();

	int pos = rCtrl.GetCurrentPos();
	int line = rCtrl.LineFromPosition(pos);
	if(lastChar !=0 && strchr("})>]", lastChar))
	{
		int startpos = rCtrl.WordStartPosition(pos-1, false);
		int linepos = rCtrl.PositionFromLine(line);
		if(startpos == linepos)
		{
			int othpos = rCtrl.BraceMatch(pos-1);
			int othline = rCtrl.LineFromPosition(othpos);
			int nIndent = rCtrl.GetLineIndentation(othline);
			char space[1024];
			memset(space,' ',1024);
			rCtrl.SetTargetStart(startpos);
			rCtrl.SetTargetEnd(pos-1);
			rCtrl.ReplaceTarget(nIndent, space);
		}
	}

	if(lastChar == '\n')
	{
		if(line > 0)
		{
			int nIndent = rCtrl.GetLineIndentation(line-1);
			int nPrevLinePos = rCtrl.PositionFromLine(line-1);
			
			int c = ' ';
			for(int p = pos-2; p>=nPrevLinePos; p--, c=rCtrl.GetCharAt(p));
			{
				if(c && strchr("{([<",c))
				{
					nIndent += 4;
				}
			}

			char space[1024];
			memset(space, ' ', 1024);
			space[nIndent] = 0;

			rCtrl.ReplaceSel(space);
		}
	}

	lastChar = 0;
}

void ParticleView::parseFile()
{
	CString pathName = GetDocument()->GetPathName();
	
	std::string extension = StringUtils::extension(std::string(pathName));
	if(extension == "particle")
	{
		parseParticleFile();
	}
}

void ParticleView::parseParticleFile()
{
	CDocument *document = GetDocument();
	ParticlePreviewPage::current->setCurrentFile(document->GetPathName());

	// Line 221
	CListBox *particleList = ParticlePreviewPage::current->getParticleList();
	particleList->ResetContent();

	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();
	for(int i=0; i<rCtrl.GetLineCount(); i++)
	{
		Ogre::String Line = rCtrl.GetLine(i);

		Ogre::StringUtil::trim(Line);
		if(Ogre::StringUtil::startsWith(Line, "particle_system "))
		{
			Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(Line, "\t\r\n: ");
			bool skipFirst = true, canPrint = true;
			std::string match, _match;
			for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
			{
				if (skipFirst)
				{
					skipFirst = false;
					continue;
				}

				std::string match = (*it);

				if(match.find_first_of('\"') != std::string::npos)
				{
					if(canPrint)
					{
						canPrint = false;
					}
					else
					{
						canPrint = true;
						match = _match + match;
					}
				}
				
				if(canPrint)
				{
					Ogre::StringUtil::trim(match);
					match.erase(0, match.find_first_not_of('\"'));  
					match.erase(match.find_last_not_of('\"')+1);  

					if(!match.empty())
					{
						particleList->InsertString(particleList->GetCount(), match.c_str());
					}
				}
				else
				{
					_match += (match + " ");
				}
			}
		}
	}
}

void ParticleView::setAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);
	if (size >= 1)
		rCtrl.StyleSetSize(style, size);
	if (face) 
		rCtrl.StyleSetFont(style, face);
}

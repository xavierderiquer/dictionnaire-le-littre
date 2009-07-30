/*
	© Murielle Descerisiers, 2009
	murielle.descerisiers@gmail.com

	Ce logiciel est un programme informatique développé au Québec et servant à
	consulter le dictionnaire de la langue française d'Émile Littré.

	Ce logiciel est régi par la licence CeCILL soumise au droit français et respectant
	les principes de diffusion des logiciels libres. Vous pouvez utiliser, modifier et/ou
	redistribuer ce programme sous les conditions de la licence CeCILL telle que diffusée
	par le CEA, le CNRS et l'INRIA sur le site http://www.cecill.info.

	En contrepartie de l'accessibilité au code source et des droits de copie, de
	modification et de redistribution accordés par cette licence, il n'est offert aux
	utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
	responsabilité restreinte pèse sur l'auteur du programme, le titulaire des droits
	patrimoniaux et les concédants successifs.

	À cet égard l'attention de l'utilisateur est attirée sur les risques associés au
	chargement, à l'utilisation, à la modification et/ou au développement et à la
	reproduction du logiciel par l'utilisateur étant donné sa spécificité de logiciel
	libre, qui peut le rendre complexe à manipuler et qui le réserve donc à des
	développeurs et des professionnels avertis possédant des connaissances informatiques
	approfondies. Les utilisateurs sont donc invités à charger et tester l'adéquation du
	logiciel à leurs besoins dans des conditions permettant d'assurer la sécurité de
	leurs systèmes et ou de leurs données et, plus généralement, à l'utiliser et
	l'exploiter dans les mêmes conditions de sécurité.

	Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
	connaissance de la licence CeCILL, et que vous en avez accepté les termes.
*/

#include "classesurligneurtypographie.h"

classeSurligneurTypographie::classeSurligneurTypographie(QTextDocument *parent) :
		QSyntaxHighlighter(parent)
{
}

void classeSurligneurTypographie::SurlignerMotsClefs(const QString &texte,
		const QStringList &listeMotsClefs, const QTextCharFormat &formatTexte)
{
	foreach(QString texteMotClef, listeMotsClefs) {
		texteMotClef = "\\b" + texteMotClef + "\\b";
		QRegExp expression(texteMotClef, Qt::CaseInsensitive);
		int nombreIndex(texte.indexOf(expression));
		while (nombreIndex >= 0) {
			const int nombreLongueur(expression.matchedLength());
			setFormat(nombreIndex, nombreLongueur, formatTexte);
			nombreIndex = texte.indexOf(expression, nombreIndex + nombreLongueur);
		}
	}
}

void classeSurligneurTypographie::highlightBlock(const QString &text)
{
	QTextCharFormat formatTexte;
	formatTexte.setFontWeight(QFont::Bold);

	QStringList listeMotsClefs;
	listeMotsClefs << "corps";
	listeMotsClefs << "prononciation";
	listeMotsClefs << QString::fromUtf8("r\303\251sum\303\251-num\303\251ro-sens");
	listeMotsClefs << QString::fromUtf8("num\303\251ro-sens");
	listeMotsClefs << "citation";
	listeMotsClefs << QString::fromUtf8("r\303\251f\303\251rence-titre");
	listeMotsClefs << QString::fromUtf8("r\303\251f\303\251rence");
	listeMotsClefs << "nature";
	listeMotsClefs << "rubrique";
	listeMotsClefs << QString::fromUtf8("suppl\303\251ment");
	listeMotsClefs << QString::fromUtf8("entr\303\251e");
	listeMotsClefs << "lien";
	listeMotsClefs << QString::fromUtf8("r\303\251sum\303\251");
	SurlignerMotsClefs(text, listeMotsClefs, formatTexte);

	formatTexte.setFontWeight(QFont::Normal);
	formatTexte.setFontItalic(true);
	listeMotsClefs.clear();
	listeMotsClefs << QString::fromUtf8("marge-sup\303\251rieure");
	listeMotsClefs << QString::fromUtf8("marge-inf303\251rieure");
	listeMotsClefs << "marge-droite";
	listeMotsClefs << "marge-gauche";
	listeMotsClefs << "marge";
	listeMotsClefs << "couleur-fond";
	listeMotsClefs << "couleur-texte";
	listeMotsClefs << QString::fromUtf8("\303\251paisseur-police");
	listeMotsClefs << "style-police";
	listeMotsClefs << "taille-police";
	listeMotsClefs << "nom-police";
	listeMotsClefs << "image-fond";
	listeMotsClefs << "effets-texte";
	SurlignerMotsClefs(text, listeMotsClefs, formatTexte);
}

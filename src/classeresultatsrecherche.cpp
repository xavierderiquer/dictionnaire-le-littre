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

#include "classeresultatsrecherche.h"

classeResultatsRecherche::classeResultatsRecherche(QObject *parent,
		QHash<int, structureEntree> *table) : QStyledItemDelegate(parent),
		tableEntrees(table), police("DejaVu Serif", 10)
{
}

QSize classeResultatsRecherche::sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	QSize taille(QStyledItemDelegate::sizeHint(option, index));
	taille.setHeight(nombreHauteurEntree);
	return taille;
}

void classeResultatsRecherche::paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	QRect rectangleTitre(option.rect);
	rectangleTitre.setHeight(nombreHauteurTitre);

	QLinearGradient degrade(rectangleTitre.topLeft(), rectangleTitre.bottomLeft());
	degrade.setColorAt(0.0, QColor::fromRgb((option.state & QStyle::State_Selected)
			? 0x7777f6 : 0x777777));
	degrade.setColorAt(0.4, QColor::fromRgb((option.state & QStyle::State_Selected)
			? 0x6060e3 : 0x606060));
	degrade.setColorAt(0.401, QColor::fromRgb((option.state & QStyle::State_Selected)
			? 0x5757da : 0x575757));
	degrade.setColorAt(1.0, QColor::fromRgb((option.state & QStyle::State_Selected)
			? 0x333394 : 0x333333));

	const int indexEntree(index.data(Qt::UserRole).toInt());
	if(indexEntree != -1) {
		painter->fillRect(rectangleTitre, QBrush(degrade));
		painter->setPen(Qt::white);
		painter->setFont(police);
		painter->drawText(rectangleTitre, Qt::AlignCenter,
				tableEntrees->value(indexEntree).texteEntree);
	}

	QTextDocument document;
	document.setUndoRedoEnabled(false);
	document.setDefaultFont(police);
	document.setTextWidth(option.rect.width());
	document.setHtml(index.data().toString());
	painter->save();
	painter->translate(rectangleTitre.bottomLeft());
	document.drawContents(painter, QRect(0, 0, option.rect.width(),
			option.rect.height() - nombreHauteurTitre));
	painter->restore();
}

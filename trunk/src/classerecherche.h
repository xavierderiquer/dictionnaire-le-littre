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

#ifndef CLASSERECHERCHE_H
#define CLASSERECHERCHE_H

#include <QDialog>
#include <QTextDocumentFragment>
#include <QCloseEvent>

#include "ui_classerecherche.h"
#include "classeminuterierouge.h"
#include "classeprincipale.h"

class classePrincipale;

class classeRecherche : public QDialog, public Ui::fenetreRecherche
{
	Q_OBJECT
public:
	classeRecherche(QWidget *parent = 0, Qt::WindowFlags f = 0);

	int nombreEntree;
	QString texteRecherche;
	bool boolArretUrgence;
private:
	QAction *actionExporterHTML,
		*actionExporterODF,
		*actionExporterTXT,
		*actionExporterPDF;
	classePrincipale *fenetrePrincipale;

	QTextDocument *CreerDocument();
	QString CreerEntete(const int nombreIndexEntree);
	void ActiverOuDesactiver(bool boolActiver, const int nombreResultats = -1);
	bool EstSusceptible(QString texte, const Qt::CaseSensitivity respecterCasse,
		const bool boolExpressionRationnelle);
	void AjouterAvertissementAuxResultats(const QString &texte);
protected:
	void closeEvent(QCloseEvent *event);
private slots:
	void Consulter();
	void Chercher(QString texteParametre = QString());
	void EvenementResultatChoisi();
	void FermerOuAnnuler();
	void ApercuImpression(QPrinter *printer);
	void AfficherApercuImpression();
	void Exporter();
	void Imprimer();
	void EvenementAccessibleZoneListe(const QModelIndex &index);
	void EvenementAccessibleAction();
};

#endif

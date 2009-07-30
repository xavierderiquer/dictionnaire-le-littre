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

#ifndef CLASSEPRINCIPALE_H
#define CLASSEPRINCIPALE_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QStringList>
#include <QRegExp>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFileDialog>
#include <QTextCodec>
#include <QPrintDialog>
#include <QPrinter>
#include <QDir>
#include <QUrl>
#include <QSystemTrayIcon>
#include <QClipboard>
#include <QScrollBar>
#include <QTextDocumentWriter>
#include <QHash>
#include <QPrintPreviewDialog>
#include <QResource>
#include <QDesktopWidget>
#include <QAccessible>

#include "ui_classeprincipale.h"
#include "classerecherche.h"
#include "classesurligneur.h"
#include "classetypographie.h"

enum {
	PLURIEL =		1,
	MASCULIN =		2,
	FEMININ =		4,
	PREMIEREPERSONNE =	8,
	TROISIEMEPERSONNE =	16,
	INDICATIF =		32,
	PARTICIPE =		64,
	PASSE =			128,
	IMPARFAIT =		256,
	CONDITIONNEL =		512,
	SUBJONCTIF =		1024,
	FUTUR =			2048,
	IMPERATIF =		4096,
	INFINITIF =		8192
};

struct structureFormeFlechie {
	QString texteForme;
	int nombreIndexEntree;
};

struct structureEntree {
	QString texteEntree;
	int nombrePositionFichier;
};

class classeSurligneur;

class classeRecherche;

class classePrincipale : public QMainWindow, public Ui::fenetrePrincipale
{
	Q_OBJECT;
public:
	QHash<int, structureEntree> tableEntrees;
	int nombrePartieEnMemoire;
	QString texteRepertoire;
	bool boolModeAccessible;

	classePrincipale(const QString &texteDossier, const QString &texteTitre,
			bool boolRemiseANeuf, QWidget *parent = 0,
			Qt::WindowFlags flags = 0);
	~classePrincipale();
	QString RecupererArticle(int nombreIndexEntree);
	int CompterFormesFlechies() { return 427704; }
	int CompterEntrees() { return 78427; }
	QString LireFormeFlechie(int nombreIndexFormeFlechie);
	QString LireEntree(int nombreIndexEntree);
	void Selectionner(QListView *boite, const int nombreIndex);
	int TrouverIndexChoisi(QListView *boite);
	QTextCursor ChercherDocument(const QString &texteRecherche, QTextDocument *document,
			QTextDocument::FindFlags optionsRecherche,
			bool boolExpressionRationnelle);
	QString XMLVersHTML(const QString &texteXML, bool boolAfficher = false);
	QString ExtraireLemme(const QString &texte);
private:
	QHash<int, structureFormeFlechie> tableFormesFlechies;

	classeRecherche *fenetreRecherche;
	classeSurligneur *surligneur;
	QAction *actionReduire,
		*actionMaximiser,
		*actionRestaurer,
		*actionPressePapiers;
	QSystemTrayIcon *icone;

	QString texteFeuilleStyle;

	void SauterBalise(QXmlStreamReader *lecteur, const QString &texteBalise);
	QString LireBalise(QXmlStreamReader *lecteur, const QString &texteBalise);
	void Naviguer(QString texteMot, bool boolIcone = false);
	void ClearHighlight();
	void ActiverOuDesactiverHistorique(const int nombreIndexSelection);
	QString TrouverTextePressePapiers();
	bool EstHAspire(const int nombreIndexEntree);
	QChar LettreSansAccent(const QChar lettre);
	bool SontEgaux(const QString &texteConsultation, const QString &texte,
			bool boolNegligerAccents);
	QString EnleverAccents(const QString &texte);
	bool EstVoyelle(QChar lettre);
	QString ConvertirTableauEnHTML(const QMultiMap<int, QString> &tableau,
			bool boolHAspire, int nombreStylePersonnel = 0);
	void RestaurerEtatDictionnaire(bool boolRemiseANeuf);
	void EnregistrerEtatDictionnaire();
	QString XMLVersTexteAccessible(const QString &texteXML);
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void changeEvent(QEvent *event);
private slots:
	void ApercuImpression(QPrinter *printer);
	void AfficherApercuImpression();
	void ChercherArticle(QString texteParametre = QString());
	void Consulter(bool boolDefilerSeulement = false);
	void EvenementChoixEntree(const QItemSelection &selected,
			const bool boolAjouterAHistorique = true);
	void EvenementHistoriqueSelection();
	void EvenementChoixSens();
	void EvenementChoixForme(const QItemSelection &selected);
	void ReculerOuAvancerHistorique();
	void Exporter();
	void ExporterBalises();
	void Imprimer();
	void EvenementModesLecture();
	void AProposDuLogiciel();
	void AfficherPortrait();
	void ChercherTousLesArticles();
	void EvenementClicLien(const QUrl &src);
	void ReculerOuAvancerEntrees();
	void AfficherPleinEcran(bool checked);
	void EvenementIcone(QSystemTrayIcon::ActivationReason reason);
	void NaviguerPressePapiers();
	void LirePreface();
	void AfficherFlexion();
	void EvenementCopieDisponible(bool yes);
	void Copier();
	void EvenementDoubleClique();
	void EvenementSaisie();
	void Restaurer();
	void ChangerTypographie();
	void EvenementAccessibleAction();
	void EvenementAccessibleZoneListe(const QModelIndex &index);
signals:
	void SignalReduireEnIcone();
	void SignalDoubleClique();
};

#endif

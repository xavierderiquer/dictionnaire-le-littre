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

#include "classeprincipale.h"
#include "classenavigation.h"
#include "classeapropos.h"
#include "classeminuterierouge.h"
#include "classeconjugaison.h"
#include "classeflexion.h"
#include "classemodeleformes.h"
#include "classemodeleentrees.h"

classePrincipale::classePrincipale(const QString &texteDossier, const QString &texteTitre,
		bool boolRemiseANeuf, QWidget *parent, Qt::WindowFlags flags)
		: QMainWindow(parent, flags), nombrePartieEnMemoire(-1),
		texteRepertoire(texteDossier), boolModeAccessible(false),
		fenetreRecherche(NULL)
{
	setupUi(this);

	RestaurerEtatDictionnaire(boolRemiseANeuf);

	actionPressePapiers = new QAction("&Presse-papiers", this);
	actionPressePapiers->setEnabled(false);
	actionPressePapiers->setIcon(boutonConsulter->icon());
	connect(actionPressePapiers, SIGNAL(triggered()), this, SLOT(NaviguerPressePapiers()));
	actionReduire = new QAction(QString::fromUtf8("&R\303\251duire"), this);
	connect(actionReduire, SIGNAL(triggered()), this, SLOT(hide()));
	actionMaximiser = new QAction("&Maximiser", this);
	connect(actionMaximiser, SIGNAL(triggered()), this, SLOT(showMaximized()));
	actionRestaurer = new QAction("R&estaurer", this);
	connect(actionRestaurer, SIGNAL(triggered()), this, SLOT(Restaurer()));
	QMenu *menuIcone = new QMenu(this);
	menuIcone->addAction(actionPressePapiers);
	menuIcone->addSeparator();
	menuIcone->addAction(actionReduire);
	menuIcone->addAction(actionMaximiser);
	menuIcone->addAction(actionRestaurer);
	menuIcone->setDefaultAction(actionRestaurer);
	menuIcone->addSeparator();
	menuIcone->addAction(actionQuitter);
	icone = new QSystemTrayIcon(windowIcon(), this);
	icone->setToolTip(windowTitle());
	icone->show();

	classeAPropos* fenetreAPropos = new classeAPropos(boolModeAccessible, this,
			Qt::SplashScreen);
	fenetreAPropos->setWindowTitle(QString::
			fromUtf8("Le dictionnaire d\303\251marre..."));
	delete fenetreAPropos->ongletPrincipal;
	delete fenetreAPropos->boutonFermer;
	fenetreAPropos->barreProgression->setRange(0, CompterEntrees()
			+ CompterFormesFlechies());
	fenetreAPropos->show();
	qApp->processEvents();

	int nombreIndexEntrees(0);

	QFile fichierBrut(":/Littre-IndexEntrees.txt");
	fichierBrut.open(QIODevice::ReadOnly);
	QTextStream fichier(&fichierBrut);
	fichier.setCodec(QTextCodec::codecForName("UTF-8"));
	while (!fichier.atEnd()) {
		structureEntree entree;
		entree.texteEntree = fichier.readLine();
		entree.nombrePositionFichier = fichier.readLine().toInt();
		tableEntrees.insert(nombreIndexEntrees, entree);
		++nombreIndexEntrees;
		if (nombreIndexEntrees % 5000 == 0) {
			fenetreAPropos->barreProgression->setValue(nombreIndexEntrees);
			qApp->processEvents();
		}
	}
	fichierBrut.close();

	int nombreIndexFormesFlechies(0);

	fichierBrut.setFileName(":/Littre-IndexFormes.txt");
	fichierBrut.open(QIODevice::ReadOnly);
	fichier.setCodec(QTextCodec::codecForName("UTF-8"));
	while (!fichier.atEnd()) {
		structureFormeFlechie forme;
		forme.texteForme = fichier.readLine();
		forme.nombreIndexEntree = fichier.readLine().toInt();
		tableFormesFlechies.insert(nombreIndexFormesFlechies, forme);
		fichier.readLine();
		++nombreIndexFormesFlechies;
		if((nombreIndexEntrees + nombreIndexFormesFlechies) % 5000 == 0) {
			fenetreAPropos->barreProgression->setValue(nombreIndexEntrees
					+ nombreIndexFormesFlechies);
			qApp->processEvents();
		}
	}
	fichierBrut.close();
	QResource::unregisterResource(texteRepertoire + "Littre-Index.dat");

	zoneListeEntrees->setModel(new classeModeleEntrees(this));
	zoneListeFormesFlechies->setModel(new classeModeleFormes(this));

	zoneTexteRechercheDansArticle->setCompleter(NULL);
	surligneur = new classeSurligneur(navigateurArticle, this);

	navigateurArticle->viewport()->installEventFilter(this);
	connect(zoneTexteRechercheDansArticle, SIGNAL(activated(QString)), this,
			SLOT(ChercherArticle(QString)));
	connect(actionOccurrenceSuivante, SIGNAL(triggered()), this, SLOT(ChercherArticle()));
	connect(actionOccurrencePrecedente, SIGNAL(triggered()), this,
			SLOT(ChercherArticle()));
	connect(actionConsulter, SIGNAL(triggered()), this, SLOT(Consulter()));
	connect(zoneTexteRechercheDansArticle, SIGNAL(editTextChanged(QString)), surligneur,
			SLOT(rehighlight()));
	connect(actionEmployerExpressionRationnelle, SIGNAL(toggled(bool)), surligneur,
			SLOT(rehighlight()));
	connect(actionRespecterCasse, SIGNAL(toggled(bool)), surligneur, SLOT(rehighlight()));
	connect(zoneListeEntrees->selectionModel(), SIGNAL
			(selectionChanged(QItemSelection,QItemSelection)),
			this, SLOT(EvenementChoixEntree(QItemSelection)));
	connect(zoneListeSurvol, SIGNAL(itemSelectionChanged()), this,
			SLOT(EvenementChoixSens()));
	connect(zoneListeFormesFlechies->selectionModel(), SIGNAL
			(selectionChanged(QItemSelection,QItemSelection)),
			this, SLOT(EvenementChoixForme(QItemSelection)));
	connect(zoneListeHistorique, SIGNAL(itemSelectionChanged()), this,
			SLOT(EvenementHistoriqueSelection()));
	connect(actionReculerHistorique, SIGNAL(triggered()), this,
			SLOT(ReculerOuAvancerHistorique()));
	connect(actionAvancerHistorique, SIGNAL(triggered()), this,
			SLOT(ReculerOuAvancerHistorique()));
	connect(actionExporterHTML, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionAfficherApercuImpression, SIGNAL(triggered()), this,
			SLOT(AfficherApercuImpression()));
	connect(actionExporterODF, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionExporterPDF, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionExporterTXT, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionExporterBalises, SIGNAL(triggered()), this, SLOT(ExporterBalises()));
	connect(actionImprimer, SIGNAL(triggered()), this, SLOT(Imprimer()));
	connect(actionInclurePrononciation, SIGNAL(toggled(bool)), this,
			SLOT(EvenementModesLecture()));
	connect(actionInclureHistorique, SIGNAL(toggled(bool)), this,
			SLOT(EvenementModesLecture()));
	connect(actionInclureEtymologie, SIGNAL(toggled(bool)), this,
			SLOT(EvenementModesLecture()));
	connect(actionInclureNature, SIGNAL(toggled(bool)), this,
			SLOT(EvenementModesLecture()));
	connect(actionInclureCitations, SIGNAL(toggled(bool)), this,
			SLOT(EvenementModesLecture()));
	connect(actionAProposDuLogiciel, SIGNAL(triggered()), this, SLOT(AProposDuLogiciel()));
	connect(actionAfficherPortrait, SIGNAL(triggered()), this, SLOT(AfficherPortrait()));
	connect(actionChercherTousLesArticles, SIGNAL(triggered()), this,
			SLOT(ChercherTousLesArticles()));
	connect(actionReculerEntrees, SIGNAL(triggered()), this,
			SLOT(ReculerOuAvancerEntrees()));
	connect(actionAvancerEntrees, SIGNAL(triggered()), this,
			SLOT(ReculerOuAvancerEntrees()));
	connect(navigateurArticle, SIGNAL(anchorClicked(const QUrl &)), this,
			SLOT(EvenementClicLien(const QUrl &)));
	connect(actionAfficherPleinEcran, SIGNAL(toggled(bool)), this,
			SLOT(AfficherPleinEcran(bool)));
	connect(icone, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(EvenementIcone(QSystemTrayIcon::ActivationReason)));
	connect(actionLirePreface, SIGNAL(triggered()), this, SLOT(LirePreface()));
	connect(actionLireCauserie, SIGNAL(triggered()), this, SLOT(LirePreface()));
	connect(actionLireTemoignageZola, SIGNAL(triggered()), this, SLOT(LirePreface()));
	connect(actionLireTemoignageRenan, SIGNAL(triggered()), this, SLOT(LirePreface()));
	connect(actionLireTemoignagePasteur, SIGNAL(triggered()), this, SLOT(LirePreface()));
	connect(actionAfficherFlexion, SIGNAL(triggered()), this, SLOT(AfficherFlexion()));
	connect(navigateurArticle, SIGNAL(copyAvailable(bool)), this,
			SLOT(EvenementCopieDisponible(bool)));
	connect(actionCopier, SIGNAL(triggered()), this, SLOT(Copier()));
	connect(this, SIGNAL(SignalReduireEnIcone()), this, SLOT(hide()),
			Qt::QueuedConnection);
	connect(this, SIGNAL(SignalDoubleClique()), this, SLOT(EvenementDoubleClique()),
			Qt::QueuedConnection);
	connect(zoneTexteConsultation, SIGNAL(textEdited(QString)), this,
			SLOT(EvenementSaisie()));
	connect(actionChangerTypographie, SIGNAL(triggered()), this,
			SLOT(ChangerTypographie()));

	boutonReculerHistorique->setDefaultAction(actionReculerHistorique);
	boutonAvancerHistorique->setDefaultAction(actionAvancerHistorique);
	boutonAfficherFlexion->setDefaultAction(actionAfficherFlexion);
	boutonCopier->setDefaultAction(actionCopier);
	boutonExporter->setMenu(menuExporter);
	boutonExporter->setIcon(menuExporter->icon());
	boutonImprimer->setDefaultAction(actionImprimer);
	boutonAgrandirTexte->setDefaultAction(actionAgrandirTexte);
	boutonReduireTexte->setDefaultAction(actionReduireTexte);
	boutonAfficherPleinEcran->setDefaultAction(actionAfficherPleinEcran);
	boutonModeConsultation->setMenu(menuModeConsultation);
	boutonModeConsultation->setToolTip(menuModeConsultation->title().replace("&", ""));
	boutonChercherTousLesArticles->setDefaultAction(actionChercherTousLesArticles);
	boutonOccurrenceSuivante->setDefaultAction(actionOccurrenceSuivante);
	boutonOccurrencePrecedente->setDefaultAction(actionOccurrencePrecedente);
	boutonModeRecherche->setMenu(menuModeRecherche);
	boutonModeRecherche->setToolTip(menuModeRecherche->title().replace("&", ""));
	boutonChampLecture->setMenu(menuChampLecture);
	boutonChampLecture->setToolTip(menuChampLecture->title().replace("&", ""));

	boolModeAccessible = QAccessible::isActive();
	setWindowTitle(texteTitre + (boolModeAccessible ? " (Mode accessible)" : ""));

	if (boolModeAccessible) {
		foreach(QComboBox *zoneTexte, findChildren<QComboBox *>())
			zoneTexte->setAccessibleName(zoneTexte->objectName());

		foreach(QLineEdit *zoneTexte, findChildren<QLineEdit *>())
			zoneTexte->setAccessibleName(zoneTexte->objectName());

		foreach(QLabel *etiquette, findChildren<QLabel *>())
			etiquette->setAccessibleName(QString::fromUtf8("\303\211tiquette ")
					+ etiquette->text().replace("&", ""));

		foreach(QToolButton *bouton, findChildren<QToolButton *>()) {
			bouton->setAccessibleName("Bouton " + bouton->toolTip()
					+ (bouton->menu() ? QString::fromUtf8(
					" (ce bouton ouvre un sous-menu)") : ""));
			bouton->setToolTip("");
		}

		foreach(QListView *zoneListe, findChildren<QListView *>()) {
			zoneListe->viewport()->setAccessibleName(zoneListe->objectName());
			zoneListe->setMouseTracking(true);
			connect(zoneListe, SIGNAL(entered(const QModelIndex &)), this, SLOT
					(EvenementAccessibleZoneListe(const QModelIndex &)));
		}

		foreach(QAction *action, findChildren<QAction *>())
			if(action->isSeparator() == false)
				connect(action, SIGNAL(hovered()), this,
						SLOT(EvenementAccessibleAction()));
	}

	actionLireCauserie->trigger();

	delete fenetreAPropos;
	icone->setContextMenu(menuIcone);
}

void classePrincipale::RestaurerEtatDictionnaire(bool boolRemiseANeuf)
{
	QFile fichierBrut(texteRepertoire + "Littre-Etat.dat");
	if(boolRemiseANeuf == false && fichierBrut.exists() &&
			fichierBrut.open(QIODevice::ReadOnly)) {
		QDataStream fichier(&fichierBrut);
		QByteArray geometrie;
		fichier >> geometrie;
		restoreGeometry(geometrie);
		if(isFullScreen())
			actionAfficherPleinEcran->toggle();
		bool boolCoche;
		fichier >> boolCoche;
		actionNegligerAccents->setChecked(boolCoche);
		fichier >> boolCoche;
		actionDefilerAFurEtAMesure->setChecked(boolCoche);
		fichier >> boolCoche;
		actionRespecterCasse->setChecked(boolCoche);
		fichier >> boolCoche;
		actionEmployerExpressionRationnelle->setChecked(boolCoche);
		fichier >> boolCoche;
		actionInclurePrononciation->setChecked(boolCoche);
		fichier >> boolCoche;
		actionInclureNature->setChecked(boolCoche);
		fichier >> boolCoche;
		actionInclureCitations->setChecked(boolCoche);
		fichier >> boolCoche;
		actionInclureHistorique->setChecked(boolCoche);
		fichier >> boolCoche;
		actionInclureEtymologie->setChecked(boolCoche);
		fichier >> texteFeuilleStyle;
	}
	else {
		QDesktopWidget bureau;
		const QRect rectangleBureau(bureau.availableGeometry(QApplication::
				desktop()->screenNumber(this)));
		const QRect rectangleFenetre(rect());
		move(rectangleBureau.center() - rectangleFenetre.center());

		texteFeuilleStyle = classeTypographie::FeuilleParDefaut();
	}
}

void classePrincipale::EvenementAccessibleAction()
{
	QAction *action(qobject_cast<QAction *>(sender()));
	QString texte(action->menu() ? "Menu " : "");
	texte += action->iconText();
	if(action->isCheckable())
		texte += QString(" (cette action ") + (action->isChecked() ? "est"
				: "n'est pas") + QString::fromUtf8(" coch\303\251e)");
	QWidget *objet = menuBarre;
	foreach(QMenu *menu, findChildren<QMenu *>())
		if(menu->actions().contains(action)) {
			if(menu->title().isEmpty() == false)
				objet = menu;
			break;
		}
	if(objet->accessibleName() != texte)
		objet->setAccessibleName(texte);
}

void classePrincipale::EvenementAccessibleZoneListe(const QModelIndex &index)
{
	QListView *zoneListe(qobject_cast<QListView *>(sender()));
	zoneListe->viewport()->setAccessibleName(ExtraireLemme(index.data().toString()));
}

void classePrincipale::EnregistrerEtatDictionnaire()
{
	QFile fichierBrut(texteRepertoire + "Littre-Etat.dat");
	if(fichierBrut.open(QFile::WriteOnly)) {
		QDataStream fichier(&fichierBrut);
		fichier << saveGeometry()
			<< actionNegligerAccents->isChecked()
			<< actionDefilerAFurEtAMesure->isChecked()
			<< actionRespecterCasse->isChecked()
			<< actionEmployerExpressionRationnelle->isChecked()
			<< actionInclurePrononciation->isChecked()
			<< actionInclureNature->isChecked()
			<< actionInclureCitations->isChecked()
			<< actionInclureHistorique->isChecked()
			<< actionInclureEtymologie->isChecked()
			<< texteFeuilleStyle;
	}
}

QString classePrincipale::LireFormeFlechie(int nombreIndexFormeFlechie)
{
	return tableFormesFlechies.value(nombreIndexFormeFlechie).texteForme;
}

QString classePrincipale::LireEntree(int nombreIndexEntree)
{
	return tableEntrees.value(nombreIndexEntree).texteEntree;
}

void classePrincipale::AfficherApercuImpression()
{
	QPrinter imprimante(QPrinter::HighResolution);
	imprimante.setDocName(navigateurArticle->documentTitle());
	QPrintPreviewDialog fenetreApercu(&imprimante, this);
	connect(&fenetreApercu, SIGNAL(paintRequested(QPrinter *)), this,
			SLOT(ApercuImpression(QPrinter *)));
	fenetreApercu.exec();
}

void classePrincipale::ChangerTypographie()
{
	classeTypographie *fenetreTypographie = new classeTypographie(boolModeAccessible,
			this, Qt::Dialog);
	fenetreTypographie->editeur->setPlainText(texteFeuilleStyle);
	if (fenetreTypographie->exec() == QDialog::Accepted &&
			(texteFeuilleStyle != fenetreTypographie->editeur->toPlainText())) {
		texteFeuilleStyle = fenetreTypographie->editeur->toPlainText();
		EvenementModesLecture();
	}
	delete fenetreTypographie;
}

void classePrincipale::ApercuImpression(QPrinter *printer)
{
	navigateurArticle->print(printer);
}

void classePrincipale::EvenementIcone(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger) {
		Restaurer();
	} else if (reason == QSystemTrayIcon::Context) {
		QString textePressePapiers(TrouverTextePressePapiers());
		if (textePressePapiers.isEmpty() == false) {
			actionPressePapiers->setText(QString::fromUtf8("Consulter \302\253 ")
				+ textePressePapiers + QString::fromUtf8(" \302\273"));
			actionPressePapiers->setEnabled(true);
		} else {
			actionPressePapiers->setText("&Presse-papiers");
			actionPressePapiers->setEnabled(false);
		}
		actionReduire->setEnabled(!isMinimized());
	}
}

int classePrincipale::TrouverIndexChoisi(QListView *boite)
{
	QModelIndexList liste(boite->selectionModel()->selectedIndexes());
	return liste.isEmpty() ? -1 : liste.first().row();
}

void classePrincipale::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::WindowStateChange && isMinimized()) {
		emit SignalReduireEnIcone();
		event->ignore();
	}
	QMainWindow::changeEvent(event);
}

classePrincipale::~classePrincipale()
{
	EnregistrerEtatDictionnaire();
	if (fenetreRecherche != NULL)
		delete fenetreRecherche;
}

void classePrincipale::EvenementClicLien(const QUrl &src)
{
	QString texteEntree(src.path().toUpper());
	const int nombreTaille(texteEntree.length());
	if (nombreTaille > 2) {
		if (texteEntree[nombreTaille - 2] == '.') {
			int nombreSens = texteEntree[nombreTaille - 1].digitValue();
			if (nombreSens != -1) {
				texteEntree.resize(nombreTaille - 2);
				texteEntree = QChar(0x2080 + nombreSens) + ' ' + texteEntree;
			}
		}
	}

	QHash<int, structureEntree>::const_iterator iterateurEntrees;
	for(iterateurEntrees = tableEntrees.constBegin();
				iterateurEntrees != tableEntrees.constEnd();
				++iterateurEntrees) {
		if(iterateurEntrees.value().texteEntree.startsWith(texteEntree)) {
			Selectionner(zoneListeEntrees, iterateurEntrees.key());
			qApp->processEvents();
			QString texteVariante(src.fragment());
			if(texteVariante.startsWith("var")) {
				texteVariante.remove(0, 3);
				bool boolReussi;
				const int nombreVariante(texteVariante.toInt(&boolReussi));
				if(boolReussi)
					zoneListeSurvol->setCurrentRow(nombreVariante - 1);
			}
			return;
		}
	}
}

void classePrincipale::Selectionner(QListView *boite, const int nombreIndex)
{
	boite->selectionModel()->setCurrentIndex(boite->model()->index(nombreIndex, 0),
		QItemSelectionModel::SelectCurrent | QItemSelectionModel::Clear);
}

bool classePrincipale::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonDblClick)
		emit SignalDoubleClique();
	return QMainWindow::eventFilter(obj, event);
}

void classePrincipale::EvenementDoubleClique()
{
	Naviguer(navigateurArticle->textCursor().selectedText());
}

QString classePrincipale::ExtraireLemme(const QString &texte)
{
	QString texteLemme;
	int nombreIndex(0);
	while ((nombreIndex < texte.length()) && (texte[nombreIndex] != ',')) {
		if (nombreIndex == 0 && texte[nombreIndex] > 0x2080) nombreIndex += 2;
		if (texte[nombreIndex] == ' ') {
			if (texte[nombreIndex+1].isLower()) // "ou", "et"
				break;
			else if (texte[nombreIndex+1] == '-')
				break;
		}
		texteLemme += texte[nombreIndex];
		++nombreIndex;
	}
	return texteLemme;
}

void classePrincipale::Naviguer(QString texteMot, bool boolIcone)
{
	texteMot = texteMot.toUpper();

	QMap<int, QString> tableauResultats;
	QHash<int, structureFormeFlechie>::const_iterator iterateurFormesFlechies;
	for(iterateurFormesFlechies = tableFormesFlechies.constBegin();
				iterateurFormesFlechies != tableFormesFlechies.constEnd();
				++iterateurFormesFlechies) {
		if(iterateurFormesFlechies.value().texteForme == texteMot) {
			tableauResultats.insert(iterateurFormesFlechies.value()
					.nombreIndexEntree, tableEntrees.value(
					iterateurFormesFlechies.value().nombreIndexEntree)
					.texteEntree);
		}
	}
	if (tableauResultats.count() > 1) {
		classeNavigation *fenetreNavigation = new classeNavigation(boolModeAccessible,
				this);
		fenetreNavigation->setWindowTitle(texteMot.toLower());
		QMap<int, QString>::const_iterator iterateur;
		for (iterateur = tableauResultats.constBegin();
				iterateur != tableauResultats.constEnd();
				++iterateur) {
			QListWidgetItem* entreeAChoisir = new QListWidgetItem(iterateur.value(),
				fenetreNavigation->zoneListeCorrespondances);
			entreeAChoisir->setData(Qt::UserRole, iterateur.key());
		}
		fenetreNavigation->zoneListeCorrespondances->setCurrentRow(0);
		fenetreNavigation->exec();
		if (fenetreNavigation->nombreEntreeChoisie != -1) {
			Selectionner(zoneListeEntrees, fenetreNavigation->nombreEntreeChoisie);
			if (boolIcone) Restaurer();
		}
		delete fenetreNavigation;
	} else if (tableauResultats.count() == 1) {
		Selectionner(zoneListeEntrees, tableauResultats.begin().key());
		if (boolIcone) Restaurer();
	} else {
		if (boolIcone)
			icone->showMessage(windowTitle(), QString::fromUtf8(
				"La forme \302\253 ") + texteMot + QString::fromUtf8(
				" \302\273 n'a pas \303\251t\303\251 trouv\303\251e."),
				QSystemTrayIcon::NoIcon);
	}
}

void classePrincipale::Restaurer()
{
	if (isHidden() || isActiveWindow())
		showNormal();
	else {
		hide();
		qApp->processEvents();
		show();
	}
}

QTextCursor classePrincipale::ChercherDocument(const QString &texteRecherche,
		QTextDocument *document, QTextDocument::FindFlags optionsRecherche,
		bool boolExpressionRationnelle)
{
	QTextCursor curseur(document);

	if (boolExpressionRationnelle) {
		QRegExp expression(texteRecherche);
		expression.setCaseSensitivity(Qt::CaseSensitivity(
				QTextDocument::FindCaseSensitively & optionsRecherche));
		curseur = document->find(expression, curseur, optionsRecherche);
	} else
		curseur = document->find(texteRecherche, curseur, optionsRecherche);
	return curseur;
}

void classePrincipale::ChercherArticle(QString texteParametre)
{
	QTextDocument *document = navigateurArticle->document();
	QString texteRecherche;
	if (sender() == actionOccurrenceSuivante || sender() == actionOccurrencePrecedente) {
		texteRecherche = zoneTexteRechercheDansArticle->currentText();
		if (zoneTexteRechercheDansArticle->findText(texteRecherche) == -1)
			zoneTexteRechercheDansArticle->addItem(texteRecherche);
	} else
		texteRecherche = texteParametre;

	if (texteRecherche.isEmpty() == false) {
		QTextDocument::FindFlags optionsRecherche = 0;
		if (sender() == actionOccurrencePrecedente)
			optionsRecherche |= QTextDocument::FindBackward;
		if (actionRespecterCasse->isChecked())
			optionsRecherche |= QTextDocument::FindCaseSensitively;
		QTextCursor curseur(ChercherDocument(texteRecherche, document,
				optionsRecherche,
				actionEmployerExpressionRationnelle->isChecked()));
		if (curseur.isNull())
			new classeMinuterieRouge(zoneTexteRechercheDansArticle);
		else
			navigateurArticle->setTextCursor(curseur);
	}
	else
		new classeMinuterieRouge(zoneTexteRechercheDansArticle);
}

QString classePrincipale::LireBalise(QXmlStreamReader *lecteur, const QString &texteBalise)
{
	QString texteResultat;
	do {
		lecteur->readNext();
		if (lecteur->isCharacters()) texteResultat += lecteur->text().toString();
	} while ((lecteur->isEndElement() && lecteur->name() == texteBalise) == false);
	return texteResultat;
}

void classePrincipale::SauterBalise(QXmlStreamReader *lecteur, const QString &texteBalise)
{
	do {
		lecteur->readNext();
	} while ((lecteur->name() == texteBalise && lecteur->isEndElement()) == false);
}

QString classePrincipale::RecupererArticle(const int nombreIndexEntree)
{
	int nombrePositionFichier(tableEntrees.value(nombreIndexEntree).nombrePositionFichier);
	const int nombrePartie(nombrePositionFichier / 10000000);
	nombrePositionFichier -= nombrePartie * 10000000;

	if(nombrePartie != nombrePartieEnMemoire) {
		if(nombrePartieEnMemoire != -1)
			QResource::unregisterResource(texteRepertoire + "Littre-Partie"
					+ QString::number(nombrePartieEnMemoire) + ".dat");
		QResource::registerResource(texteRepertoire + "Littre-Partie"
				+ QString::number(nombrePartie) + ".dat");
		nombrePartieEnMemoire = nombrePartie;
	}
	QFile fichierBrut(":/Littre-Partie" + QString::number(nombrePartie) + ".txt");
	fichierBrut.open(QIODevice::ReadOnly);
	QTextStream fichier(&fichierBrut);
	fichier.setCodec(QTextCodec::codecForName("UTF-8"));
	fichier.seek(nombrePositionFichier);
	QString texteResultat;
	do {
		const QString texte(fichier.readLine());
		texteResultat += texte + "\n";
		if (texte == "</entree>") break;
	} while (!fichier.atEnd());
	fichierBrut.close();
	return texteResultat;
}

QString classePrincipale::XMLVersHTML(const QString &texteXML, bool boolAfficher)
{
	if(boolAfficher)
		zoneListeSurvol->clear();

	QStringList listeHTML;
	listeHTML << "<html>";
	listeHTML << "<style>" + classeTypographie::FeuilleVersCSS(texteFeuilleStyle)
			+ "</style>";
	listeHTML << "<body>";
	QXmlStreamReader lecteur(texteXML);
	QString texteSensCourant;
	bool boolLireSens(false);
	while (!lecteur.atEnd()) {
		lecteur.readNext();

		if (lecteur.isEndElement()) {
			if (lecteur.name() == "entete")
				listeHTML << "<p><p>";
		} else if (lecteur.isStartElement()) {
			if (lecteur.name() == "entree") {
				if (lecteur.attributes().value("supplement") == "1") {
					listeHTML << QString::fromUtf8("<div align=\"center\" "
							"class=\"supplement\">SUPPL\303\211MEN"
							"T AU DICTIONNAIRE</div>");
					listeHTML << "<p>";
				}
				listeHTML << "<span class=\"entree\">";
				QString variante(lecteur.attributes().value("sens").toString());
				if (variante.isEmpty() == false)
					variante = QChar(0x2080 + variante.toInt())
							+ QString(" ");
				listeHTML.last() = listeHTML.last() + variante + lecteur
						.attributes().value("terme").toString()
						+ "</span>";
			} else if(lecteur.name() == QString::fromUtf8("r\303\251sum\303\251")) {
				QString texteResume("<div class=\"resume\">");
				do {
					lecteur.readNext();
					if (lecteur.isStartElement() && lecteur.name()
							== "variante") {
						const QString texteNumero(lecteur.attributes()
								.value("num").toString());
						if (texteNumero.isEmpty() == false)
							texteResume += QString::fromUtf8("<spa"
									"n class=\"resumenumer"
									"o\">") + texteNumero
									+ QString::fromUtf8(
									"\302\260\302\240"
									"</span>");
						texteResume += lecteur.text().toString();
					}
					else if (lecteur.isCharacters())
						texteResume += lecteur.text().toString();
				} while ((lecteur.isEndElement() && lecteur.name() ==
						QString::fromUtf8("r\303\251sum\303\251"))
						== false);
				texteResume += "</div>";
				listeHTML << texteResume;
			} else if (lecteur.name() == "prononciation") {
				if (actionInclurePrononciation->isChecked())
					listeHTML << "<span class=\"prononciation\">["
					+ LireBalise(&lecteur, "prononciation")
					+ "]</span>";
				else SauterBalise(&lecteur, "prononciation");
			} else if (lecteur.name() == "nature") {
				if (actionInclureNature->isChecked())
					listeHTML << "<span class=\"nature\">"
							+ LireBalise(&lecteur, "nature")
							+ "</span>";
				else
					SauterBalise(&lecteur, "nature");
			} else if (lecteur.name() == "rubrique") {
				const QString texteNom(lecteur.attributes().value("nom")
						.toString().toUpper());
				if (((texteNom == "HISTORIQUE" && actionInclureHistorique->
						isChecked() == false) || (texteNom == QString::
						fromUtf8("\303\211TYMOLOGIE") &&
						actionInclureEtymologie->isChecked() == false))
						== false) {
					listeHTML << QString("<div align=\"center\" class=\"")
							+ ((texteNom == QString::fromUtf8("SUP"
							"PL\303\211MENT AU DICTIONNAIRE"))
							? "supplement" : "rubrique") + "\">"
							+ texteNom + "</div>";
					listeHTML << "<p>";
				} else SauterBalise(&lecteur, "rubrique");
			} else if (lecteur.name() == "variante") {
				const QString texteNumero(lecteur.attributes().value("num")
						.toString());
				if (texteNumero.isEmpty() == false) {
					listeHTML << "<a name=\"" + texteNumero + QString::
							fromUtf8("\"></a><p><span class=\""
							"numero\">\302\240") + texteNumero
							+ QString::fromUtf8("\302\260\302\240<"
							"/span> ");
					if(boolAfficher) {
						texteSensCourant = texteNumero + QString::
								fromUtf8("\302\260  ");
						boolLireSens = true;
					}
				} else
					listeHTML << "<p style=\"text-indent:20px;\">";
			} else if (lecteur.name() == "a") {
				QString texteLien(lecteur.attributes().value("ref").toString());
				texteLien = "<a href=\"" + texteLien + QString::fromUtf8("\" c"
						"lass=\"lien\">\342\244\267")
						+ LireBalise(&lecteur, "a").replace(" ",
						QString::fromUtf8("\302\240")) + "</a>";
				listeHTML.last() = listeHTML.last() + texteLien;
			} else if (lecteur.name() == "indent") {
				listeHTML << "<p style=\"text-indent:20px;\">";
			} else if (lecteur.name() == "cit") {
				if (actionInclureCitations->isChecked()) {
					const QString texteAuteur(lecteur.attributes()
							.value("aut").toString());
					const QString texteReferenceTitre(lecteur.attributes()
							.value("ref").toString());
					QString texte(QString::fromUtf8(" \342\231\246\302\240"
							"<span class=\"citation\">"));
					texte += LireBalise(&lecteur, "cit");
					texte += ", </span><span class=\"reference\">";
					if (texteAuteur.isEmpty() == false)
						texte += texteAuteur;
					if (texteReferenceTitre.isEmpty() == false) {
						if (texteAuteur.isEmpty() == false)
							texte += ", ";
						texte += "<span class=\"referencetitre\">"
							 + texteReferenceTitre + "</span>";
					}
					texte += "</span> ";
					listeHTML.last() = listeHTML.last() + texte;
				} else
					SauterBalise(&lecteur, "cit");
			}
		} else if (lecteur.isWhitespace() == false) {
			listeHTML.last() = listeHTML.last() + lecteur.text().toString();
			if (boolAfficher && boolLireSens) {
				QString texte(lecteur.text().toString().trimmed());
				if(texte.endsWith(","))
					texte.resize(texte.length() - 1);
				if(texte.endsWith(", voy."))
					texte.resize(texte.length() - 6);
				if(texte.startsWith(". ") || texte.startsWith(", "))
					texte.remove(0, 2);
				new QListWidgetItem(texteSensCourant + texte, zoneListeSurvol);
				boolLireSens = false;
			}
		}

	}
	listeHTML << "</body></html>";
	return listeHTML.join("\n");
}

QString classePrincipale::XMLVersTexteAccessible(const QString &texteXML)
{
	QStringList listeTexte;
	listeTexte << QString::fromUtf8("D\303\211BUT ARTICLE. ");
	QXmlStreamReader lecteur(texteXML);
	while (!lecteur.atEnd()) {
		lecteur.readNext();

		if (lecteur.isEndElement()) {
			if (lecteur.name() == "entete")
				listeTexte << "" << "";
		} else if (lecteur.isStartElement()) {
			if (lecteur.name() == "entree") {
				QString variante(lecteur.attributes().value("sens")
						 .toString());
				if (variante.isEmpty() == false)
					variante += " ";
				listeTexte.last() = listeTexte.last() + variante + lecteur
						.attributes().value("terme").toString() + ".";
			} else if(lecteur.name() == QString::fromUtf8(
						"r\303\251sum\303\251")) {
				QString texteResume(QString::fromUtf8(" D\303\211BUT R\303\211"
						"SUM\303\211. "));
				do {
					lecteur.readNext();
					if (lecteur.isStartElement() && lecteur.name()
							== "variante") {
						const QString texteNumero(lecteur.attributes()
								.value("num").toString());
						if (texteNumero.isEmpty() == false)
							texteResume += texteNumero + ". ";
						texteResume += lecteur.text().toString();
					}
					else if (lecteur.isCharacters())
						texteResume += lecteur.text().toString();
				} while ((lecteur.isEndElement() && lecteur.name() == QString::
						fromUtf8("r\303\251sum\303\251")) == false);
				texteResume += QString::fromUtf8(" FIN R\303\211SUM\303\211. ");
				listeTexte << texteResume;
			} else if (lecteur.name() == "prononciation") {
				if (actionInclurePrononciation->isChecked())
					listeTexte << QString::fromUtf8(" D\303\211BUT PRONONC"
						"IATION. ") + LireBalise(&lecteur,
						"prononciation") + " FIN PRONONCIATION. ";
				else SauterBalise(&lecteur, "prononciation");
			} else if (lecteur.name() == "nature") {
				if (actionInclureNature->isChecked())
					listeTexte << QString::fromUtf8(" D\303\211BUT NATURE"
							". ") + LireBalise(&lecteur, "nature")
							+ QString::fromUtf8(" FIN NATURE. ");
				else
					SauterBalise(&lecteur, "nature");
			} else if (lecteur.name() == "rubrique") {
				const QString texteNom(lecteur.attributes().value("nom")
						.toString().toUpper());
				if (((texteNom == "HISTORIQUE" && actionInclureHistorique->
						isChecked() == false) || (texteNom == QString::
						fromUtf8("\303\211TYMOLOGIE") &&
						actionInclureEtymologie->isChecked() == false))
						== false) {
					listeTexte << " RUBRIQUE " + texteNom + ". ";
					listeTexte << "";
				} else SauterBalise(&lecteur, "rubrique");
			} else if (lecteur.name() == "variante") {
				const QString texteNumero(lecteur.attributes().value("num")
						.toString());
				if (texteNumero.isEmpty() == false) {
					listeTexte << QString::fromUtf8(" SENS ")
							+ texteNumero + ". ";
				} else
					listeTexte << "";
			} else if (lecteur.name() == "a") {
				listeTexte.last() = listeTexte.last() +
						LireBalise(&lecteur, "a");
			} else if (lecteur.name() == "indent") {
				listeTexte << "";
			} else if (lecteur.name() == "cit") {
				if (actionInclureCitations->isChecked()) {
					const QString texteAuteur(lecteur.attributes()
							.value("aut").toString());
					QString texte(QString::fromUtf8(" D\303\211BUT "
							"CITATION. "));
					texte += LireBalise(&lecteur, "cit");
					if (texteAuteur.isEmpty() == false)
						texte += ". " + texteAuteur + ".";
					texte += " FIN CITATION. ";
					listeTexte.last() = listeTexte.last() + texte;
				} else
					SauterBalise(&lecteur, "cit");
			}
		} else if (lecteur.isWhitespace() == false) {
			listeTexte.last() = listeTexte.last() + lecteur.text().toString();
		}

	}
	listeTexte << " FIN ARTICLE. ";
	return listeTexte.join("\n");
}

bool classePrincipale::EstVoyelle(QChar lettre)
{
	lettre = LettreSansAccent(lettre);
	return (lettre == 'E' || lettre == 'Y' || lettre == 'U' || lettre == 'I'
			|| lettre == 'O' || lettre == 'A');
}

QString classePrincipale::ConvertirTableauEnHTML(const QMultiMap<int, QString> &tableau,
		bool boolHAspire, int nombreStylePersonnel)
{
	QStringList listePersonnes;

	if (nombreStylePersonnel == 0)
		listePersonnes << "je" << "tu" << "il, elle" << "nous" << "vous" << "ils, elles";
	else if (nombreStylePersonnel == 1)
		listePersonnes << "que je" << "que tu" << "qu'il, qu'elle" << "que nous"
				<< "que vous" << "qu'ils, qu'elles";
	QStringList listeHTML("<html><body><table align=\"center\">");

	QMap<int, QString>::const_iterator iterateur;
	for (iterateur = tableau.constBegin(); iterateur != tableau.constEnd(); ++iterateur) {
		if (nombreStylePersonnel == 0 || nombreStylePersonnel == 1) {
			QString textePersonne(listePersonnes[iterateur.key()-1]);
			if (iterateur.key()-1 == 0) { // je
				if (EstVoyelle(iterateur.value()[0]) || (boolHAspire == false
						&& iterateur.value()[0] == 'H'))
					textePersonne[textePersonne.length()-1] = '\'';
			}
			listeHTML << "<tr><td style=\"color:darkslategray;\" align=\"right\">"
					+ textePersonne + "&nbsp;</td><td>" + iterateur.value()
					+ "</td></tr>";
		} else if (nombreStylePersonnel == 2)
			listeHTML << "<tr><td align=\"center\">" + iterateur.value()
					+ "</td></tr>";
		else if (nombreStylePersonnel == 3) {
			QString texteNature;
			if (iterateur.key() == 0)
				texteNature = "singulier";
			else if (iterateur.key() == 1)
				texteNature = "singulier masculin";
			else if (iterateur.key() == 2)
				texteNature = QString::fromUtf8("singulier f\303\251minin");
			else if (iterateur.key() == 3)
				texteNature = "pluriel";
			else if (iterateur.key() == 4)
				texteNature = "pluriel masculin";
			else if (iterateur.key() == 5)
				texteNature = QString::fromUtf8("pluriel f\303\251minin");
			listeHTML << "<tr><td style=\"color:darkslategray;\" align=\"right\">"
				+ texteNature + "&nbsp;</td><td>" + iterateur.value()
				+ "</td></tr>";
		}
	}
	listeHTML << "</table></body></html>";
	return listeHTML.join("\n");
}

bool classePrincipale::EstHAspire(const int nombreIndexEntree)
{
	const QString texte(RecupererArticle(nombreIndexEntree));
	QXmlStreamReader lecteur(texte);
	while (!lecteur.atEnd()) {
		lecteur.readNext();
		if (lecteur.isStartElement() && lecteur.name() == "prononciation")
			return (LireBalise(&lecteur, "prononciation").startsWith("h"));
	}
	return false;
}

void classePrincipale::AfficherFlexion()
{
	int nombreIndexEntreeChoisie(TrouverIndexChoisi(zoneListeEntrees));

	QList<int> listeFormesFlechiesCorrespondantes;

	QHash<int, structureFormeFlechie>::const_iterator iterateurFormesFlechies;
	for(iterateurFormesFlechies = tableFormesFlechies.constBegin();
			iterateurFormesFlechies != tableFormesFlechies.constEnd();
			++iterateurFormesFlechies)
		if(iterateurFormesFlechies.value().nombreIndexEntree
				== nombreIndexEntreeChoisie)
			listeFormesFlechiesCorrespondantes << iterateurFormesFlechies.key();

	qSort(listeFormesFlechiesCorrespondantes);

	QList<int> listeNatures;

	QResource::registerResource(texteRepertoire + "Littre-Index.dat");
	QFile fichierBrut(":/Littre-IndexFormes.txt");
	fichierBrut.open(QIODevice::ReadOnly);
	QTextStream fichier(&fichierBrut);
	fichier.setCodec(QTextCodec::codecForName("UTF-8"));
	for (int nombreIndex=0; nombreIndex<listeFormesFlechiesCorrespondantes.count();
			++nombreIndex) {
		int nombreLignePrecedente = (nombreIndex == 0) ? 0
				: listeFormesFlechiesCorrespondantes[nombreIndex-1] * 3 + 3;

		for (int nombreLignesASauter = nombreLignePrecedente; nombreLignesASauter
				< (listeFormesFlechiesCorrespondantes[nombreIndex] * 3 + 2);
				++nombreLignesASauter)
			fichier.readLine();
		listeNatures << fichier.readLine().toInt();
	}
	fichierBrut.close();
	QResource::unregisterResource(texteRepertoire + "Littre-Index.dat");

	if (actionAfficherFlexion->text() != "Afficher la fle&xion...") {
		QMultiMap<int, QString> tableauIndicatifPresent, tableauIndicatifImparfait,
		tableauIndicatifPasseSimple, tableauIndicatifFuturSimple,
		tableauSubjonctifPresent, tableauSubjonctifImparfait,
		tableauConditionnelPresent, tableauImperatifPresent, tableauParticipePresent,
		tableauParticipePasse;

		for (int nombreIndex = 0; nombreIndex < listeFormesFlechiesCorrespondantes
				.count(); ++nombreIndex) {
			const int nombreNature = listeNatures.at(nombreIndex);
			int nombrePersonne = (nombreNature & PLURIEL) ? 3 : 0;
			if(nombreNature & PARTICIPE) {
				if(nombreNature & MASCULIN) ++nombrePersonne;
				else if(nombreNature & FEMININ) nombrePersonne += 2;
			} else {
				if(nombreNature & PREMIEREPERSONNE) ++nombrePersonne;
				else if(nombreNature & TROISIEMEPERSONNE) nombrePersonne += 3;
				else nombrePersonne += 2;
			}
			QMultiMap<int, QString> *map;
			if(nombreNature & INDICATIF) {
				if(nombreNature & PASSE)
					map = &tableauIndicatifPasseSimple;
				else if(nombreNature & FUTUR)
					map = &tableauIndicatifFuturSimple;
				else if(nombreNature & IMPARFAIT)
					map = &tableauIndicatifImparfait;
				else
					map = &tableauIndicatifPresent;
			} else if(nombreNature & SUBJONCTIF) {
				if(nombreNature & IMPARFAIT)
					map = &tableauSubjonctifImparfait;
				else
					map = &tableauSubjonctifPresent;
			} else if(nombreNature & CONDITIONNEL)
				map = &tableauConditionnelPresent;
			else if(nombreNature & IMPERATIF)
				map = &tableauImperatifPresent;
			else if(nombreNature & PARTICIPE)  {
				if(nombreNature & PASSE)
					map = &tableauParticipePasse;
				else
					map = &tableauParticipePresent;
			}
			else
				continue;

			map->insert(nombrePersonne, tableFormesFlechies.value(
				listeFormesFlechiesCorrespondantes[nombreIndex]).texteForme);
		}

		const bool boolHAspire = navigateurArticle->documentTitle().startsWith('h') ?
			EstHAspire(nombreIndexEntreeChoisie) : false;

		classeConjugaison *fenetreConjugaison = new classeConjugaison(
				boolModeAccessible, this, Qt::Dialog);
		fenetreConjugaison->setWindowTitle(navigateurArticle->documentTitle());
		fenetreConjugaison->navigateurIndicatifPresent->setHtml(
				ConvertirTableauEnHTML(tableauIndicatifPresent, boolHAspire));
		fenetreConjugaison->navigateurIndicatifImparfait->setHtml(
				ConvertirTableauEnHTML(tableauIndicatifImparfait,
				boolHAspire));
		fenetreConjugaison->navigateurIndicatifPasseSimple->setHtml(
				ConvertirTableauEnHTML(tableauIndicatifPasseSimple,
				boolHAspire));
		fenetreConjugaison->navigateurIndicatifFuturSimple->setHtml(
				ConvertirTableauEnHTML(tableauIndicatifFuturSimple,
				boolHAspire));
		fenetreConjugaison->navigateurSubjonctifPresent->setHtml(
				ConvertirTableauEnHTML(tableauSubjonctifPresent,
				boolHAspire, 1));
		fenetreConjugaison->navigateurSubjonctifImparfait->setHtml(
				ConvertirTableauEnHTML(tableauSubjonctifImparfait,
				boolHAspire, 1));
		fenetreConjugaison->navigateurConditionnelPresent->setHtml(
				ConvertirTableauEnHTML(tableauConditionnelPresent,
				boolHAspire));
		fenetreConjugaison->navigateurImperatifPresent->setHtml(
				ConvertirTableauEnHTML(tableauImperatifPresent,
				boolHAspire, 2));
		fenetreConjugaison->navigateurParticipePresent->setHtml(
				ConvertirTableauEnHTML(tableauParticipePresent,
				boolHAspire, 2));
		fenetreConjugaison->navigateurParticipePasse->setHtml(
				ConvertirTableauEnHTML(tableauParticipePasse,
				boolHAspire, 3));
		if(boolModeAccessible)
			foreach(QTextBrowser *navigateur, fenetreConjugaison->
					findChildren<QTextBrowser *>())
				navigateur->viewport()->setAccessibleName(
						navigateur->toPlainText());
		fenetreConjugaison->exec();

		delete fenetreConjugaison;

	} else {
		QMultiMap<int, QString> tableauFormes;
		for (int nombreIndex = 0; nombreIndex < listeFormesFlechiesCorrespondantes
				.count(); ++nombreIndex) {
			const int nombreNature = listeNatures.at(nombreIndex);
			int nombrePersonne = (nombreNature & PLURIEL) ? 3 : 0;
			if(nombreNature & MASCULIN) ++nombrePersonne;
			else if(nombreNature & FEMININ) nombrePersonne += 2;
			tableauFormes.insert(nombrePersonne, tableFormesFlechies.value(
					listeFormesFlechiesCorrespondantes[nombreIndex])
					.texteForme);
		}
		classeFlexion *fenetreFlexion = new classeFlexion(boolModeAccessible, this,
				Qt::Dialog);
		fenetreFlexion->setWindowTitle(navigateurArticle->documentTitle());
		fenetreFlexion->navigateurFormes->setHtml(
				ConvertirTableauEnHTML(tableauFormes, false, 3));
		if(boolModeAccessible)
			foreach(QTextBrowser *navigateur, fenetreFlexion->
					findChildren<QTextBrowser *>())
				navigateur->viewport()->setAccessibleName(
						navigateur->toPlainText());
		fenetreFlexion->exec();
		delete fenetreFlexion;
	}
}

int NatureFlexible(const QString &texteNature)
{
	if (texteNature.startsWith("s") || texteNature.startsWith("adj")
			|| texteNature.startsWith("p.") || texteNature.startsWith("part"))
		return 0;
	if (texteNature.startsWith("v"))
		return 1;
	return -1;
}

void classePrincipale::EvenementChoixEntree(const QItemSelection &selected,
		const bool boolAjouterAHistorique)
{
	const int nombreIndexEntreeChoisie = selected.indexes().first().row();
	if (menuExporterBalises->isEnabled() == false)
		menuExporterBalises->setEnabled(true);
	actionReculerEntrees->setEnabled(nombreIndexEntreeChoisie > 0);
	actionAvancerEntrees->setEnabled(nombreIndexEntreeChoisie < (CompterEntrees() - 1));
	const QString texteXML(RecupererArticle(nombreIndexEntreeChoisie));
	navigateurArticle->setHtml(XMLVersHTML(texteXML, true));
	navigateurArticle->setDocumentTitle(ExtraireLemme(tableEntrees.value(
			nombreIndexEntreeChoisie).texteEntree).toLower());
	if(boolModeAccessible) {
		const QString texteAccessible(XMLVersTexteAccessible(texteXML));
		navigateurArticle->viewport()->setAccessibleName(texteAccessible);
		navigateurArticle->setAccessibleName(texteAccessible);
		zoneListeEntrees->viewport()->setAccessibleName(navigateurArticle->
				documentTitle());
	}

	bool boolActiverFlexion(false);

	int nombreCompteFormesFlechies(0);
	QHash<int, structureFormeFlechie>::const_iterator iterateurFormesFlechies;
	for(iterateurFormesFlechies = tableFormesFlechies.constBegin();
			iterateurFormesFlechies != tableFormesFlechies.constEnd();
			++iterateurFormesFlechies) {
		if(iterateurFormesFlechies.value().nombreIndexEntree
				== nombreIndexEntreeChoisie) {
			++nombreCompteFormesFlechies;
			if (nombreCompteFormesFlechies == 2) {
				int nombrePosition = tableEntrees.value(
						nombreIndexEntreeChoisie).texteEntree
						.indexOf(" - ");
				if (nombrePosition > 1) {
					const QString texteNature(tableEntrees.value(
							nombreIndexEntreeChoisie).texteEntree
							.right(tableEntrees.value(
							nombreIndexEntreeChoisie).texteEntree
							.length() - nombrePosition - 3));
					const int nombreNature(NatureFlexible(texteNature));
					if (nombreNature == 0) {
						actionAfficherFlexion->setText(
								"Afficher la fle&xion...");
						boolActiverFlexion = true;
					} else if (nombreNature == 1) {
						actionAfficherFlexion->setText(
								"Afficher la con&jugaison...");
						boolActiverFlexion = true;
					}
				}
				break;
			}
		}
	}

	if(boolActiverFlexion == false)
		actionAfficherFlexion->setText("Afficher la fle&xion...");
	actionAfficherFlexion->setEnabled(boolActiverFlexion);

	if(boolModeAccessible) {
		boutonAfficherFlexion->setAccessibleName("Bouton "
				+ boutonAfficherFlexion->text().replace("&", ""));
		boutonAfficherFlexion->setToolTip("");
	}

	if (boolAjouterAHistorique) {
		disconnect(zoneListeHistorique, SIGNAL(itemSelectionChanged()), this,
				SLOT(EvenementHistoriqueSelection()));
		int nombreSelectionHistorique(zoneListeHistorique->currentRow());
		if (nombreSelectionHistorique > 0)
			for (int nombreIndexHistorique = nombreSelectionHistorique - 1;
					nombreIndexHistorique >= 0; --nombreIndexHistorique)
				delete zoneListeHistorique->item(nombreIndexHistorique);
		QListWidgetItem *entreeHistorique = new QListWidgetItem(tableEntrees.value(
				nombreIndexEntreeChoisie).texteEntree);
		entreeHistorique->setData(Qt::UserRole, nombreIndexEntreeChoisie);
		zoneListeHistorique->insertItem(0, entreeHistorique);
		zoneListeHistorique->setCurrentRow(0);
		connect(zoneListeHistorique, SIGNAL(itemSelectionChanged()), this,
				SLOT(EvenementHistoriqueSelection()));
		ActiverOuDesactiverHistorique(0);
	}
}

void classePrincipale::ActiverOuDesactiverHistorique(const int nombreIndexSelection)
{
	actionReculerHistorique->setEnabled(zoneListeHistorique->count() >
			(nombreIndexSelection + 1));
	actionAvancerHistorique->setEnabled(nombreIndexSelection > 0);
	if(boolModeAccessible) {
		boutonReculerHistorique->setToolTip("");
		boutonAvancerHistorique->setToolTip("");
	}
}
void classePrincipale::EvenementSaisie()
{
	if(actionDefilerAFurEtAMesure->isChecked())
		Consulter(true);
}

QChar classePrincipale::LettreSansAccent(const QChar lettre)
{
	if (lettre >= 0x00C8 && lettre <= 0x00CB)
		return 'E';
	if (lettre >= 0x00C0 && lettre <= 0x00C6)
		return 'A';
	if (lettre >= 0x00CC && lettre <= 0x00CF)
		return 'I';
	if (lettre >= 0x00D2 && lettre <= 0x00D8)
		return 'O';
	if (lettre >= 0x00D9 && lettre <= 0x00DC)
		return 'U';
	if (lettre == 0x00C7)
		return 'C';
	return lettre;
}

QString classePrincipale::EnleverAccents(const QString &texte)
{
	QString texteResultat;
	foreach(QChar lettre, texte)
		texteResultat += LettreSansAccent(lettre);
	return texteResultat;
}

bool classePrincipale::SontEgaux(const QString &texteConsultation, const QString &texte,
		bool boolNegligerAccents)
{
	int nombreDecalage = (texte[0] > 0x2080) ? 2 : 0;
	int nombreTaille(texteConsultation.length());
	if((texte.length() - nombreDecalage) < nombreTaille)
		return false;
	for(int nombreIndex = 0; nombreIndex < nombreTaille; ++nombreIndex) {
		if(boolNegligerAccents == false) {
			if(texte[nombreIndex + nombreDecalage]
					!= texteConsultation[nombreIndex])
				return false;
		} else {
			if(LettreSansAccent(texte[nombreIndex + nombreDecalage])
					!= texteConsultation[nombreIndex])
				return false;
		}
	}
	return true;
}

void classePrincipale::Consulter(bool boolDefilerSeulement)
{
	const bool boolNegligerAccents = actionNegligerAccents->isChecked();
	QString texteConsultation(zoneTexteConsultation->text().trimmed());
	if (texteConsultation.isEmpty() == false) {
		texteConsultation = texteConsultation.toUpper();
		if(boolNegligerAccents)
			texteConsultation = EnleverAccents(texteConsultation);
		if (ongletNomenclature->currentWidget() == pageEntrees) {
			QHash<int, structureEntree>::const_iterator iterateurEntrees;
			for(iterateurEntrees = tableEntrees.constBegin();
					iterateurEntrees != tableEntrees.constEnd();
					++iterateurEntrees) {
				if(SontEgaux(texteConsultation, iterateurEntrees.value()
						.texteEntree, boolNegligerAccents)) {
					if(boolDefilerSeulement)
						zoneListeEntrees->scrollTo(zoneListeEntrees->
								model()->index(iterateurEntrees
								.key(), 0), QAbstractItemView::
								PositionAtTop);
					else if (iterateurEntrees.key() != TrouverIndexChoisi(
							zoneListeEntrees))
						Selectionner(zoneListeEntrees,
								iterateurEntrees.key());
					return;
				}
			}
		} else {
			QHash<int, structureFormeFlechie>::const_iterator
					iterateurFormesFlechies;
			for(iterateurFormesFlechies = tableFormesFlechies.constBegin();
					iterateurFormesFlechies != tableFormesFlechies
					.constEnd(); ++iterateurFormesFlechies) {
				if(SontEgaux(texteConsultation, iterateurFormesFlechies.value()
						.texteForme, boolNegligerAccents)) {
					if(boolDefilerSeulement)
						zoneListeFormesFlechies->scrollTo(
								zoneListeFormesFlechies->
								model()->index(
								iterateurFormesFlechies.key(),
								0), QAbstractItemView::
								PositionAtTop);
					else if (iterateurFormesFlechies.key() !=
							TrouverIndexChoisi(
							zoneListeFormesFlechies))
						Selectionner(zoneListeFormesFlechies,
								iterateurFormesFlechies.key());
					return;
				}
			}
		}
	}
	if(boolDefilerSeulement == false)
		new classeMinuterieRouge(zoneTexteConsultation);
}

void classePrincipale::ReculerOuAvancerHistorique()
{
	int nombreSelectionHistorique(zoneListeHistorique->currentRow());
	if (nombreSelectionHistorique < 0) nombreSelectionHistorique = 0;
	nombreSelectionHistorique += (sender() == actionReculerHistorique) ? 1 : -1;
	zoneListeHistorique->setCurrentRow(nombreSelectionHistorique);
}

void classePrincipale::Exporter()
{
	QString texteFiltre;
	QStringList listeExtensions;
	if(sender() == actionExporterODF) {
		texteFiltre = "Fichiers OpenDocument (*.odf)";
		listeExtensions << "odf";
	}
	else if(sender() == actionExporterTXT) {
		texteFiltre = "Fichiers texte simple (*.txt)";
		listeExtensions << "txt";
	}
	else if(sender() == actionExporterPDF) {
		texteFiltre = "Fichiers PDF (*.pdf)";
		listeExtensions << "pdf";
	}
	else {
		texteFiltre = "Fichiers HTML (*.html *.htm)";
		listeExtensions << "html" << "htm";
	}

	QString texteNomFichier = QFileDialog::getSaveFileName(this,
			menuExporter->title().replace("&", ""),
			QString(), texteFiltre + ";;Tous les fichiers (*)");

	if (texteNomFichier.isEmpty() == false) {
		if(listeExtensions.contains(QFileInfo(texteNomFichier).suffix().toLower())
				== false)
			texteNomFichier += "." + listeExtensions.at(0);

		if(listeExtensions.at(0) == "pdf") {
			QPrinter imprimate(QPrinter::HighResolution);
			imprimate.setDocName(navigateurArticle->documentTitle());
			imprimate.setOutputFormat(QPrinter::PdfFormat);
			imprimate.setOutputFileName(texteNomFichier);
			navigateurArticle->print(&imprimate);
		}
		else {
			QTextDocumentWriter exporteur(texteNomFichier);
			exporteur.setFormat(listeExtensions.at(0).toAscii());
			if (exporteur.write(navigateurArticle->document()) == false)
				QMessageBox::warning(this, windowTitle(), QString::fromUtf8(
						"Impossible d'\303\251crire le fichier ")
						+ texteNomFichier + ".");
		}
	}
}

void classePrincipale::Imprimer()
{
	QPrinter imprimante(QPrinter::HighResolution);
	imprimante.setDocName(navigateurArticle->documentTitle());
	QPrintDialog *fenetreImprimer = new QPrintDialog(&imprimante, this);
	if (navigateurArticle->textCursor().hasSelection())
		fenetreImprimer->addEnabledOption(QAbstractPrintDialog::PrintSelection);
	fenetreImprimer->setWindowTitle(actionImprimer->iconText());
	if (fenetreImprimer->exec() == QDialog::Accepted)
		navigateurArticle->print(&imprimante);
	delete fenetreImprimer;
}

void classePrincipale::EvenementModesLecture()
{
	const int nombreIndexEntree(TrouverIndexChoisi(zoneListeEntrees));
	if(nombreIndexEntree != -1) {
		QItemSelection selection;
		selection.select(zoneListeEntrees->model()->index(nombreIndexEntree, 0),
			zoneListeEntrees->model()->index(nombreIndexEntree, 0));
		EvenementChoixEntree(selection, false);
	}
}

void classePrincipale::EvenementHistoriqueSelection()
{
	const int nombreIndexHistorique(zoneListeHistorique->currentRow());
	int nombreIndexEntree(zoneListeHistorique->item(nombreIndexHistorique)->
			data(Qt::UserRole).toInt());
	disconnect(zoneListeEntrees->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this,
			SLOT(EvenementChoixEntree(QItemSelection)));
	Selectionner(zoneListeEntrees, nombreIndexEntree);
	QItemSelection selection;
	selection.select(zoneListeEntrees->model()->index(nombreIndexEntree, 0),
		zoneListeEntrees->model()->index(nombreIndexEntree, 0));
	EvenementChoixEntree(selection, false);
	ActiverOuDesactiverHistorique(nombreIndexHistorique);
	connect(zoneListeEntrees->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this,
			SLOT(EvenementChoixEntree(QItemSelection)));
}

void classePrincipale::AProposDuLogiciel()
{
	classeAPropos* fenetreAPropos = new classeAPropos(boolModeAccessible, this,
			Qt::Dialog);
	delete fenetreAPropos->barreProgression;
	delete fenetreAPropos->etiquetteFond;
	fenetreAPropos->exec();
	delete fenetreAPropos;
}

void classePrincipale::AfficherPortrait()
{
	classeAPropos* fenetreAPropos = new classeAPropos(boolModeAccessible, this,
			Qt::Dialog);
	fenetreAPropos->setWindowTitle(QString::fromUtf8("Portrait d'\303\211mile "
			"Littr\303\251"));
	delete fenetreAPropos->barreProgression;
	delete fenetreAPropos->ongletPrincipal;
	fenetreAPropos->exec();
	delete fenetreAPropos;
}

void classePrincipale::ChercherTousLesArticles()
{
	if (fenetreRecherche == NULL)
		fenetreRecherche = new classeRecherche(this, Qt::Dialog);
	fenetreRecherche->nombreEntree = -1;
	fenetreRecherche->exec();
	if (fenetreRecherche->nombreEntree != -1) {
		Selectionner(zoneListeEntrees, fenetreRecherche->nombreEntree);
		ChercherArticle(fenetreRecherche->texteRecherche);
	}
}

void classePrincipale::ExporterBalises()
{
	QString texteFiltre = "Fichiers XML (*.xml)";
	QStringList listeExtensions;
	listeExtensions << "xml";

	QString texteNomFichier = QFileDialog::getSaveFileName(this,
		menuExporterBalises->title().replace("&", ""),
		QString(), texteFiltre + ";;Tous les fichiers (*)");

	if (texteNomFichier.isEmpty() == false) {
		if(listeExtensions.contains(QFileInfo(texteNomFichier).suffix().toLower())
				== false)
			texteNomFichier += "." + listeExtensions.at(0);

		QFile fichierBrut(texteNomFichier);
		if (fichierBrut.open(QFile::WriteOnly) == false) {
			QMessageBox::warning(this, windowTitle(), QString::fromUtf8(
					"Impossible d'\303\251crire le fichier ")
					+ texteNomFichier + ".");
			return;
		}
		QTextStream fichier(&fichierBrut);
		fichier.setCodec(QTextCodec::codecForName("UTF-8"));
		const int nombreIndexEntreeChoisie(TrouverIndexChoisi(zoneListeEntrees));
		fichier << RecupererArticle(nombreIndexEntreeChoisie);
	}
}

void classePrincipale::ReculerOuAvancerEntrees()
{
	int nombreIndexEntrees(TrouverIndexChoisi(zoneListeEntrees));
	nombreIndexEntrees += (sender() == actionReculerEntrees) ? -1 : 1;
	Selectionner(zoneListeEntrees, nombreIndexEntrees);
}

void classePrincipale::AfficherPleinEcran(bool checked)
{
	if (checked == false)
		showNormal();
	else
		showFullScreen();
}

void classePrincipale::EvenementChoixSens()
{
	QString texteSens(zoneListeSurvol->currentItem()->text());
	texteSens.resize(texteSens.indexOf(QString::fromUtf8("\302\260  "), 0,
			Qt::CaseSensitive));
	navigateurArticle->scrollToAnchor(texteSens);
}

QString classePrincipale::TrouverTextePressePapiers()
{
	QClipboard *pressePapiers = QApplication::clipboard();
	QString texte = (pressePapiers->supportsSelection()) ?
		pressePapiers->text(QClipboard::Selection) :
		pressePapiers->text();
	texte = texte.simplified().toLower();
	texte.truncate(20);
	texte.replace(QChar(0x0152), "OE");
	return texte;
}

void classePrincipale::NaviguerPressePapiers()
{
	Naviguer(TrouverTextePressePapiers(), true);
}

void classePrincipale::LirePreface()
{
	QResource::registerResource(texteRepertoire + "Littre-Prefaces.dat");
	QString texteFichier, texteTitre;
	if(sender() == actionLirePreface) {
		texteFichier = "Littre-Preface.html";
		texteTitre = QString::fromUtf8("pr\303\251face");
	} else if(sender() == actionLireCauserie) {
		texteFichier = "Littre-Causerie.html";
		texteTitre = "causerie";
	} else if(sender() == actionLireTemoignageZola) {
		texteFichier = "Littre-Zola.html";
		texteTitre = "Zola";
	} else if(sender() == actionLireTemoignageRenan) {
		texteFichier = "Littre-Renan.html";
		texteTitre = "Renan";
	} else if(sender() == actionLireTemoignagePasteur) {
		texteFichier = "Littre-Pasteur.html";
		texteTitre = "Pasteur";
	} else {
		QResource::unregisterResource(texteRepertoire + "Littre-Prefaces.dat");
		return;
	}
	QFile fichierBrut(":/" + texteFichier);
	fichierBrut.open(QIODevice::ReadOnly);
	QTextStream fichier(&fichierBrut);
	fichier.setCodec(QTextCodec::codecForName("UTF-8"));
	navigateurArticle->setHtml(fichier.readAll());
	if(boolModeAccessible) {
		const QString texteAccessible(navigateurArticle->toPlainText());
		navigateurArticle->viewport()->setAccessibleName(texteAccessible);
		navigateurArticle->setAccessibleName(texteAccessible);
	}
	navigateurArticle->setDocumentTitle(texteTitre);
	QResource::unregisterResource(texteRepertoire + "Littre-Prefaces.dat");
}

void classePrincipale::EvenementChoixForme(const QItemSelection &selected)
{
	Selectionner(zoneListeEntrees, tableFormesFlechies.value(selected.indexes().first()
			.row()).nombreIndexEntree);
}

void classePrincipale::EvenementCopieDisponible(bool yes)
{
	QString texte = yes ? QString::fromUtf8("&Copier la s\303\251lection")
		: "&Copier le texte entier";
	actionCopier->setText(texte);

	if(boolModeAccessible) {
		boutonCopier->setAccessibleName("Bouton " + boutonCopier->toolTip());
		boutonCopier->setToolTip("");
	}
}

void classePrincipale::Copier()
{
	if (actionCopier->text() == "&Copier le texte entier") {
		navigateurArticle->setUpdatesEnabled(false);
		const int nombreAncienneVerticale(navigateurArticle->
				verticalScrollBar()->value());
		const int nombreAncienneHorizontale(navigateurArticle->
				horizontalScrollBar()->value());
		const QTextCursor curseurAncien(navigateurArticle->textCursor());
		QTextCursor curseurNouveau(curseurAncien);
		curseurNouveau.select(QTextCursor::Document);
		navigateurArticle->setTextCursor(curseurNouveau);
		navigateurArticle->copy();
		navigateurArticle->setTextCursor(curseurAncien);
		navigateurArticle->verticalScrollBar()->setValue(nombreAncienneVerticale);
		navigateurArticle->horizontalScrollBar()->setValue(nombreAncienneHorizontale);
		navigateurArticle->setUpdatesEnabled(true);
	} else
		navigateurArticle->copy();
}

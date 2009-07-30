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

#include "classerecherche.h"
#include "classeresultatsrecherche.h"

classeRecherche::classeRecherche(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f),
		boolArretUrgence(false), fenetrePrincipale(
				qobject_cast<classePrincipale*>(parent))
{
	setupUi(this);
	setFixedSize(size());

	barreProgression->setMaximum(fenetrePrincipale->CompterEntrees());

	QMenu *menuModeChamp = new QMenu(this);
	menuModeChamp->addAction(fenetrePrincipale->actionRespecterCasse);
	menuModeChamp->addAction(fenetrePrincipale->actionEmployerExpressionRationnelle);
	menuModeChamp->addSeparator();
	menuModeChamp->addAction(fenetrePrincipale->actionInclurePrononciation);
	menuModeChamp->addAction(fenetrePrincipale->actionInclureNature);
	menuModeChamp->addAction(fenetrePrincipale->actionInclureCitations);
	menuModeChamp->addAction(fenetrePrincipale->actionInclureHistorique);
	menuModeChamp->addAction(fenetrePrincipale->actionInclureEtymologie);
	boutonModeChamp->setMenu(menuModeChamp);

	barreProgression->setVisible(false);
	boutonAfficherApercuImpression->setVisible(false);
	boutonImprimer->setVisible(false);
	boutonExporter->setVisible(false);

	zoneListeResultats->setItemDelegate(
			new classeResultatsRecherche(this, &fenetrePrincipale->tableEntrees));

	zoneTexteRecherche->setCompleter(NULL);
	connect(zoneTexteRecherche, SIGNAL(activated(QString)), this, SLOT(Chercher(QString)));
	connect(boutonChercher, SIGNAL(clicked()), this, SLOT(Chercher()));

	connect(zoneListeResultats, SIGNAL(activated(QModelIndex)), this, SLOT(Consulter()));
	connect(boutonConsulter, SIGNAL(clicked()), this, SLOT(Consulter()));
	connect(zoneListeResultats, SIGNAL(itemSelectionChanged()), this,
			SLOT(EvenementResultatChoisi()));
	connect(boutonFermerOuAnnuler, SIGNAL(clicked()), this, SLOT(FermerOuAnnuler()));

	QMenu *menuExporter(new QMenu(this));
	classePrincipale *fenetrePrincipale = qobject_cast<classePrincipale *>(parent);
	actionExporterHTML = new QAction(fenetrePrincipale->actionExporterHTML->text(), this);
	actionExporterODF = new QAction(fenetrePrincipale->actionExporterODF->text(), this);
	actionExporterTXT = new QAction(fenetrePrincipale->actionExporterTXT->text(), this);
	actionExporterPDF = new QAction(fenetrePrincipale->actionExporterPDF->text(), this);
	menuExporter->addAction(actionExporterHTML);
	menuExporter->addAction(actionExporterODF);
	menuExporter->addAction(actionExporterPDF);
	menuExporter->addAction(actionExporterTXT);
	connect(actionExporterHTML, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionExporterODF, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionExporterPDF, SIGNAL(triggered()), this, SLOT(Exporter()));
	connect(actionExporterTXT, SIGNAL(triggered()), this, SLOT(Exporter()));
	boutonExporter->setMenu(menuExporter);
	connect(boutonImprimer, SIGNAL(clicked()), this, SLOT(Imprimer()));
	connect(boutonAfficherApercuImpression, SIGNAL(clicked()), this,
			SLOT(AfficherApercuImpression()));

	if(fenetrePrincipale->boolModeAccessible) {
		foreach(QComboBox *zoneTexte, findChildren<QComboBox *>())
			zoneTexte->setAccessibleName(zoneTexte->objectName());

		foreach(QLabel *etiquette, findChildren<QLabel *>())
			etiquette->setAccessibleName(QString::fromUtf8("\303\211tiquette ")
					+ etiquette->text().replace("&", ""));

		foreach(QPushButton *bouton, findChildren<QPushButton *>())
			bouton->setAccessibleName("Bouton " + bouton->text().replace("&", ""));

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
}

void classeRecherche::ActiverOuDesactiver(bool boolActiver, const int nombreResultats)
{
	boutonFermerOuAnnuler->setText(boolActiver ? "&Fermer"
			: QString::fromUtf8("&Arr\303\252ter"));
	if(fenetrePrincipale->boolModeAccessible)
		boutonFermerOuAnnuler->setAccessibleName("Bouton " + boutonFermerOuAnnuler->text());
	zoneTexteRecherche->setVisible(boolActiver);
	boutonModeChamp->setVisible(boolActiver);
	boutonChercher->setVisible(boolActiver);
	barreProgression->setVisible(!boolActiver);
	boutonConsulter->setVisible(boolActiver);

	if(boolActiver && nombreResultats == 0)
		boolActiver = false;
	boutonAfficherApercuImpression->setVisible(boolActiver);
	boutonImprimer->setVisible(boolActiver);
	boutonExporter->setVisible(boolActiver);
}

void classeRecherche::FermerOuAnnuler()
{
	if(boutonConsulter->isVisible())
		close();
	else
		boolArretUrgence = true;
}

void classeRecherche::closeEvent(QCloseEvent *event)
{
	if (boutonConsulter->isVisible())
		event->accept();
	else
		event->ignore();
}

bool classeRecherche::EstSusceptible(QString texte, const Qt::CaseSensitivity respecterCasse,
		const bool boolExpressionRationnelle)
{
	texte.replace(QRegExp("<[^>]*>"), "");
	const QRegExp::PatternSyntax syntaxe(boolExpressionRationnelle ? QRegExp::RegExp
			: QRegExp::FixedString);
	const QRegExp expression(texteRecherche, respecterCasse, syntaxe);
	return texte.contains(expression);
}

void classeRecherche::AjouterAvertissementAuxResultats(const QString &texte)
{
	QListWidgetItem* entreeResultat = new QListWidgetItem("<p align=\"center\" style=\"col"
			"or:darkslategray;\">" + texte + "</p>", zoneListeResultats);
	entreeResultat->setData(Qt::UserRole, -1);
}

void classeRecherche::Chercher(QString texteParametre)
{
	texteRecherche = (sender() == zoneTexteRecherche) ? texteParametre
			 : zoneTexteRecherche->currentText();
	if (texteRecherche.trimmed().isEmpty())
		new classeMinuterieRouge(zoneTexteRecherche);
	else {
		zoneListeResultats->clear();
		zoneListeResultats->setEnabled(false);
		boutonConsulter->setEnabled(false);

		ActiverOuDesactiver(false);
		qApp->processEvents();

		const Qt::CaseSensitivity respecterCasse(Qt::CaseSensitivity(
				fenetrePrincipale->actionRespecterCasse->isChecked()));
		const bool boolExpressionRationnelle(fenetrePrincipale->
				actionEmployerExpressionRationnelle->isChecked());

		QHash<int, structureEntree>::const_iterator iterateurEntrees;
		QFile *fichierBrut = NULL;
		QTextStream *fichier = NULL;

		for(iterateurEntrees = fenetrePrincipale->tableEntrees.constBegin();
				iterateurEntrees != fenetrePrincipale->tableEntrees.constEnd();
				++iterateurEntrees) {
			if(boolArretUrgence) {
				ActiverOuDesactiver(true, zoneListeResultats->count());
				delete fichier;
				delete fichierBrut;
				AjouterAvertissementAuxResultats(QString::fromUtf8("La recherc"
						"he a \303\251t\303\251 interrompue."));
				boolArretUrgence = false;
				return;
			}

			int nombrePositionFichier = iterateurEntrees.value().nombrePositionFichier;
			const int nombrePartie = nombrePositionFichier / 10000000;
			nombrePositionFichier -= nombrePartie * 10000000;

			if(nombrePartie != fenetrePrincipale->nombrePartieEnMemoire) {
				if(fenetrePrincipale->nombrePartieEnMemoire != -1)
					QResource::unregisterResource(fenetrePrincipale->
							texteRepertoire + "Littre-Partie"
							+ QString::number(fenetrePrincipale->
							nombrePartieEnMemoire) + ".dat");

				delete fichier;
				fichier = NULL;
				delete fichierBrut;

				QResource::registerResource(fenetrePrincipale->
						texteRepertoire + "Littre-Partie"
						+ QString::number(nombrePartie) + ".dat");
				fenetrePrincipale->nombrePartieEnMemoire = nombrePartie;
			}
			if(fichier == NULL) {
				fichierBrut = new QFile(":/Littre-Partie"
						+ QString::number(nombrePartie) + ".txt");
				fichierBrut->open(QIODevice::ReadOnly);
				fichier = new QTextStream(fichierBrut);
				fichier->setCodec(QTextCodec::codecForName("UTF-8"));
			}
			fichier->seek(nombrePositionFichier);
			bool boolSusceptible(false);
			QString texteResultat;
			do {
				const QString texte(fichier->readLine());
				if(boolSusceptible == false)
					boolSusceptible = EstSusceptible(texte, respecterCasse,
							boolExpressionRationnelle);
				texteResultat += texte;
				if (texte == "</entree>") break;
			} while (!fichier->atEnd());

			if(boolSusceptible) {
				texteResultat = fenetrePrincipale->XMLVersHTML(texteResultat);
				QTextDocument document;
				document.setUndoRedoEnabled(false);
				document.setHtml(texteResultat);
				const QTextDocument::FindFlags optionsRecherche(
						(respecterCasse == Qt::CaseInsensitive)
						? 0 : QTextDocument::FindCaseSensitively);
				QTextCursor curseur(fenetrePrincipale->ChercherDocument(
						texteRecherche, &document, optionsRecherche,
						boolExpressionRationnelle));
				if(curseur.isNull() == false) {
					QTextCharFormat formatTexte;
					formatTexte.setBackground(Qt::yellow);
					curseur.mergeCharFormat(formatTexte);
					curseur.movePosition(QTextCursor::WordLeft,
							QTextCursor::MoveAnchor, 10);
					curseur.movePosition(QTextCursor::Right,
							QTextCursor::KeepAnchor);
					const QChar caractere(curseur.selectedText().at(0));
					if(caractere.isPunct()) {
						curseur.movePosition(QTextCursor::WordLeft,
								QTextCursor::MoveAnchor);
						curseur.movePosition(QTextCursor::WordLeft,
								QTextCursor::MoveAnchor);
					}
					curseur.movePosition(QTextCursor::WordRight,
							QTextCursor::KeepAnchor, 50);
					QTextDocumentFragment fragment(curseur);
					QListWidgetItem* entreeResultat = new QListWidgetItem(
							fragment.toHtml(), zoneListeResultats);
					entreeResultat->setData(Qt::UserRole,
							iterateurEntrees.key());
					const int nombreResultats(zoneListeResultats->count());
					if(nombreResultats == 1)
						zoneListeResultats->setEnabled(true);
					else if(nombreResultats == 1000) {
						ActiverOuDesactiver(true, nombreResultats);
						delete fichier;
						delete fichierBrut;
						AjouterAvertissementAuxResultats(QString::
								fromUtf8("Seuls les mille prem"
								"iers r\303\251sultats sont af"
								"fich\303\251s."));
						return;
					}
				}
			}

			if(iterateurEntrees.key() % 500 == 0) {
				barreProgression->setValue(iterateurEntrees.key());
				if(fenetrePrincipale->boolModeAccessible)
					barreProgression->setAccessibleName("Progression "
							+ barreProgression->text());
				qApp->processEvents();
				zoneListeResultats->repaint();
			}
		}
		delete fichier;
		delete fichierBrut;
		const int nombreResultats(zoneListeResultats->count());
		ActiverOuDesactiver(true, nombreResultats);
		if(nombreResultats == 0)
			AjouterAvertissementAuxResultats(QString::fromUtf8("Aucun r\303\251sul"
					"tat n'a \303\251t\303\251 trouv\303\251."));
	}
}

void classeRecherche::Consulter()
{
	if(boutonConsulter->isEnabled() && boutonConsulter->isVisible()) {
		nombreEntree = zoneListeResultats->currentItem()->data(Qt::UserRole).toInt();
		close();
	}
}

void classeRecherche::EvenementResultatChoisi()
{
	const int nombreIndexResultatChoisi(zoneListeResultats->currentRow());
	boutonConsulter->setEnabled (nombreIndexResultatChoisi != -1 && zoneListeResultats->
			item(nombreIndexResultatChoisi)->data(Qt::UserRole).toInt() != -1);
}

QString classeRecherche::CreerEntete(const int nombreIndexEntree)
{
	const QString texte("<br><div style=\"font-family:'DejaVu Serif'; font-size:12pt;"
			"font-weight: bold;\">" + fenetrePrincipale->tableEntrees.value(
			nombreIndexEntree).texteEntree + "</div> ");
	return texte;
}

QTextDocument *classeRecherche::CreerDocument()
{
	QTextDocument *document = new QTextDocument(this);
	QTextCursor curseur(document);
	const QString texte(QString::fromUtf8("<div style=\"font-family:'DejaVu Serif'; "
			"font-size:14pt; font-weight: bold;\" align=\"center\">R\303\251sultat"
			"s correspondant \303\240 <i>") + texteRecherche + "</i></div><br>");
	curseur.insertHtml(texte);
	for(int nombreIndex = 0; nombreIndex < zoneListeResultats->count(); ++nombreIndex) {
		QListWidgetItem *resultat(zoneListeResultats->item(nombreIndex));
		curseur.insertHtml(CreerEntete(resultat->data(Qt::UserRole).toInt()));
		curseur.insertHtml(resultat->text());
	}
	return document;
}

void classeRecherche::Exporter()
{
	QTextDocument *document = CreerDocument();

	QString texteFiltre;
	QStringList listeExtensions;
	if(sender() == actionExporterODF) {
		texteFiltre = "Fichiers OpenDocument (*.odf)";
		listeExtensions << "odf";
	} else if(sender() == actionExporterTXT) {
		texteFiltre = "Fichiers texte simple (*.txt)";
		listeExtensions << "txt";
	} else if(sender() == actionExporterPDF) {
		texteFiltre = "Fichiers PDF (*.pdf)";
		listeExtensions << "pdf";
	} else {
		texteFiltre = "Fichiers HTML (*.html *.htm)";
		listeExtensions << "html" << "htm";
	}

	QString texteNomFichier = QFileDialog::getSaveFileName(this,
			boutonExporter->text().replace("&", ""),
			QString(), texteFiltre + ";;Tous les fichiers (*)");

	if (texteNomFichier.isEmpty() == false) {
		if(listeExtensions.contains(
				QFileInfo(texteNomFichier).suffix().toLower()) == false)
			texteNomFichier += "." + listeExtensions.at(0);

		if(listeExtensions.at(0) == "pdf") {
			QPrinter imprimate(QPrinter::HighResolution);
			imprimate.setDocName(QString::fromUtf8("r\303\251sultats"));
			imprimate.setOutputFormat(QPrinter::PdfFormat);
			imprimate.setOutputFileName(texteNomFichier);
			document->print(&imprimate);
		}
		else {
			QTextDocumentWriter exporteur(texteNomFichier);
			exporteur.setFormat(listeExtensions.at(0).toAscii());
			if (exporteur.write(document) == false)
				QMessageBox::warning(this, fenetrePrincipale->windowTitle(),
						QString::fromUtf8("Impossible d'\303\251"
						"crire le fichier ") + texteNomFichier + ".");
		}
	}

	delete document;
}

void classeRecherche::Imprimer()
{
	QTextDocument *document = CreerDocument();
	QPrinter imprimante(QPrinter::HighResolution);
	imprimante.setDocName(QString::fromUtf8("r\303\251sultats"));
	QPrintDialog *fenetreImprimer = new QPrintDialog(&imprimante, this);
	fenetreImprimer->setWindowTitle(boutonImprimer->text().replace("&", ""));
	if (fenetreImprimer->exec() == QDialog::Accepted)
		document->print(&imprimante);
	delete fenetreImprimer;
	delete document;
}

void classeRecherche::AfficherApercuImpression()
{
	QPrinter imprimante(QPrinter::HighResolution);
	imprimante.setDocName(QString::fromUtf8("r\303\251sultats"));
	QPrintPreviewDialog fenetreApercu(&imprimante, this);
	connect(&fenetreApercu, SIGNAL(paintRequested(QPrinter *)), this,
			SLOT(ApercuImpression(QPrinter *)));
	fenetreApercu.exec();
}

void classeRecherche::ApercuImpression(QPrinter *printer)
{
	QTextDocument *document = CreerDocument();
	document->print(printer);
	delete document;
}

void classeRecherche::EvenementAccessibleAction()
{
	QAction *action(qobject_cast<QAction *>(sender()));
	QString texte(action->iconText());
	if(action->isCheckable())
		texte += QString(" (cette action ") + (action->isChecked() ? "est"
				: "n'est pas") + QString::fromUtf8(" coch\303\251e)");
	foreach(QMenu *menu, findChildren<QMenu *>())
		if(menu->actions().contains(action)) {
			if(menu->accessibleName() != texte)
				menu->setAccessibleName(texte);;
			break;
		}
}

void classeRecherche::EvenementAccessibleZoneListe(const QModelIndex &index)
{
	QListView *zoneListe(qobject_cast<QListView *>(sender()));
	QString texteAccessible;
	const int indexEntree(index.data(Qt::UserRole).toInt());
	if(indexEntree != -1)
		texteAccessible += QString::fromUtf8("ENTR\303\211E ") + fenetrePrincipale->
				ExtraireLemme(fenetrePrincipale->LireEntree(indexEntree))
				+ ". ";
	QTextDocument document;
	document.setHtml(index.data().toString());
	texteAccessible += document.toPlainText();
	zoneListe->viewport()->setAccessibleName(texteAccessible);
}

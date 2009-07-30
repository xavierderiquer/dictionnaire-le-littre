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

#include <QApplication>
#include <QTranslator>
#include <QFontDatabase>

#include "classestyle.h"
#include "classeprincipale.h"

QString AjouterBarreOblique(const QString &texteChemin)
{
	return texteChemin.endsWith(QDir::separator()) ? texteChemin
		: texteChemin + QDir::separator();
}

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.setStyle(new classeStyle);


	QFileInfo fichier("Littre.qm");
	QString texteRepertoire(fichier.exists()
			? AjouterBarreOblique(fichier.absolutePath())
			: AjouterBarreOblique(application.applicationDirPath()));

	QString texteTitre(QString::fromUtf8("Dictionnaire le Littr\303\251"));
	bool boolRemiseANeuf(false);

	for(int nombreArgument = 0; nombreArgument < argc; ++nombreArgument) {
		if(QString(argv[nombreArgument]) == "--titre" && nombreArgument < (argc - 1))
			texteTitre = QString::fromLocal8Bit(argv[nombreArgument + 1]);
		else if(QString(argv[nombreArgument]) == "--repertoire"
				&& nombreArgument < (argc - 1))
			texteRepertoire = AjouterBarreOblique(
					QString::fromLocal8Bit(argv[nombreArgument + 1]));
		else if(QString(argv[nombreArgument]) == "--remise-a-neuf")
			boolRemiseANeuf = true;
	}

	QTranslator traducteur;
	if (traducteur.load("Littre", texteRepertoire) == false) {
		QMessageBox::warning(0, texteTitre, "Impossible d'ouvrir le fichier "
				+ texteRepertoire + QString::fromUtf8("Littre.qm. L'applic"
				"ation ne peut d\303\251marrer sans ce fichier."));
		return 0;
	}
	application.installTranslator(&traducteur);

	QResource::registerResource(texteRepertoire + "Littre-Index.dat");

	QStringList listeFichiersPolice;
	listeFichiersPolice << "DejaVuSerif.ttf" << "DejaVuSerif-Bold.ttf"
			<< "DejaVuSerif-Italic.ttf" << "DejaVuSerif-BoldItalic.ttf";
	for (QStringList::const_iterator iterateurPolice = listeFichiersPolice.constBegin();
			iterateurPolice != listeFichiersPolice.constEnd(); ++iterateurPolice) {
		QFile fichier(":/" + *iterateurPolice);
		if (fichier.open(QIODevice::ReadOnly))
			QFontDatabase::addApplicationFontFromData(fichier.readAll());
	}
	application.setFont(QFont("DejaVu Serif", 10));

	classePrincipale fenetrePrincipale(texteRepertoire, texteTitre, boolRemiseANeuf);
	fenetrePrincipale.show();
	return application.exec();
}

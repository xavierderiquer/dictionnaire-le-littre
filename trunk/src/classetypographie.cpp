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

#include "classetypographie.h"
#include "classesurligneurtypographie.h"

classeTypographie::classeTypographie(bool boolModeAccessible, QWidget *parent,
		Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	setFixedSize(size());
	QPalette palette(this->palette());
	palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
	navigateur->setPalette(palette);

	connect(boutonRemettreANeuf, SIGNAL(clicked()), this, SLOT(RemettreANeuf()));

	new classeSurligneurTypographie(editeur->document());

	if(boolModeAccessible) {
		const QString texteAccessibleNavigateur(navigateur->toPlainText());
		navigateur->viewport()->setAccessibleName(texteAccessibleNavigateur);
		navigateur->setAccessibleName(texteAccessibleNavigateur);
		editeur->viewport()->setAccessibleName(editeur->toPlainText());

		foreach(QPushButton *bouton, findChildren<QPushButton *>())
			bouton->setAccessibleName("Bouton " + bouton->text().replace("&", ""));
	}
}

void classeTypographie::RemplacerMotEntier(QString &texte,
		const QString &texteMotDeRemplacement, QString texteMot)
{
	texteMot = "\\b" + texteMot + "\\b";
	const QRegExp expression(texteMot, Qt::CaseInsensitive);
	texte.replace(expression, texteMotDeRemplacement);
}

void classeTypographie::RemettreANeuf()
{
	editeur->setPlainText(FeuilleParDefaut());
}

QString classeTypographie::FeuilleVersCSS(QString texteFeuille)
{
	RemplacerMotEntier(texteFeuille, "body", "corps");
	RemplacerMotEntier(texteFeuille, "span.prononciation", "prononciation");
	RemplacerMotEntier(texteFeuille, "span.resumenumero", QString::fromUtf8(
			"r\303\251sum\303\251-num\303\251ro-sens"));
	RemplacerMotEntier(texteFeuille, "span.numero", QString::fromUtf8(
			"num\303\251ro-sens"));
	RemplacerMotEntier(texteFeuille, "span.citation", "citation");
	RemplacerMotEntier(texteFeuille, "span.referencetitre", QString::fromUtf8(
			"r\303\251f\303\251rence-titre"));
	RemplacerMotEntier(texteFeuille, "span.reference", QString::fromUtf8(
			"r\303\251f\303\251rence"));
	RemplacerMotEntier(texteFeuille, "span.nature", "nature");
	RemplacerMotEntier(texteFeuille, "div.rubrique", "rubrique");
	RemplacerMotEntier(texteFeuille, "div.supplement", QString::fromUtf8(
			"suppl\303\251ment"));
	RemplacerMotEntier(texteFeuille, "span.entree", QString::fromUtf8("entr\303\251e"));
	RemplacerMotEntier(texteFeuille, "a.lien", "lien");
	RemplacerMotEntier(texteFeuille, "div.resume", QString::fromUtf8(
			"r\303\251sum\303\251"));

	RemplacerMotEntier(texteFeuille, "margin-top", QString::fromUtf8(
			"marge-sup\303\251rieure"));
	RemplacerMotEntier(texteFeuille, "margin-bottom", QString::fromUtf8(
			"marge-inf303\251rieure"));
	RemplacerMotEntier(texteFeuille, "margin-right", "marge-droite");
	RemplacerMotEntier(texteFeuille, "margin-left", "marge-gauche");
	RemplacerMotEntier(texteFeuille, "margin", "marge");
	RemplacerMotEntier(texteFeuille, "background-color", "couleur-fond");
	RemplacerMotEntier(texteFeuille, "background-image", "image-fond");
	RemplacerMotEntier(texteFeuille, "color", "couleur-texte");
	RemplacerMotEntier(texteFeuille, "font-weight", QString::fromUtf8(
			"\303\251paisseur-police"));
	RemplacerMotEntier(texteFeuille, "font-style", "style-police");
	RemplacerMotEntier(texteFeuille, "font-size", "taille-police");
	RemplacerMotEntier(texteFeuille, "font-family", "nom-police");
	RemplacerMotEntier(texteFeuille, "text-decoration", "effets-texte");

	RemplacerMotEntier(texteFeuille, "bold", "gras");
	RemplacerMotEntier(texteFeuille, "italic", "italique");
	RemplacerMotEntier(texteFeuille, "small", "petite");
	RemplacerMotEntier(texteFeuille, "medium", "moyenne");
	RemplacerMotEntier(texteFeuille, "xx-large", QString::fromUtf8(
			"tr\303\250s-tr\303\250s-grande"));
	RemplacerMotEntier(texteFeuille, "x-large", QString::fromUtf8("tr\303\250s-grande"));
	RemplacerMotEntier(texteFeuille, "none", "aucun");
	RemplacerMotEntier(texteFeuille, "underline", QString::fromUtf8("soulign\303\251"));
	RemplacerMotEntier(texteFeuille, "overline", QString::fromUtf8("surlign\303\251"));
	RemplacerMotEntier(texteFeuille, "line-through", QString::fromUtf8("barr\303\251"));

	RemplacerMotEntier(texteFeuille, "lightgoldenrodyellow", "jaune-paille-clair");
	RemplacerMotEntier(texteFeuille, "darkseagreen", QString::fromUtf8(
			"vert-oc\303\251an-sombre"));
	RemplacerMotEntier(texteFeuille, "mediumvioletred", QString::fromUtf8(
			"rouge-violac\303\251-moyen"));
	RemplacerMotEntier(texteFeuille, "lightsteelblue", "bleu-acier-clair");
	RemplacerMotEntier(texteFeuille, "palegoldenrod", QString::fromUtf8(
			"jaune-paille-p\303\242le"));
	RemplacerMotEntier(texteFeuille, "darkslateblue", "bleu-ardoise-sombre");
	RemplacerMotEntier(texteFeuille, "sienna", "terre-de-sienne");
	RemplacerMotEntier(texteFeuille, "rosybrown", "bois-de-rose");
	RemplacerMotEntier(texteFeuille, "mediumaquamarine", "aigue-marine-moyen");
	RemplacerMotEntier(texteFeuille, "mediumspringgreen", "vert-printemps-moyen");
	RemplacerMotEntier(texteFeuille, "darkgoldenrod", "jaune-paille-sombre");
	RemplacerMotEntier(texteFeuille, "linen", "blanc-de-lin");
	RemplacerMotEntier(texteFeuille, "deepskyblue", "bleu-ciel-profond");
	RemplacerMotEntier(texteFeuille, "olivedrab", "vert-olive-terne");
	RemplacerMotEntier(texteFeuille, "palevioletred", QString::fromUtf8(
			"rouge-violac\303\251-p\303\242le"));
	RemplacerMotEntier(texteFeuille, "lightslategray", "gris-ardoise-clair");
	RemplacerMotEntier(texteFeuille, "mediumslateblue", "bleu-ardoise-moyen");
	RemplacerMotEntier(texteFeuille, "cornsilk", QString::fromUtf8(
			"jaune-ma\303\257s-doux"));
	RemplacerMotEntier(texteFeuille, "darkslategray", "gris-ardoise-sombre");
	RemplacerMotEntier(texteFeuille, "lightskyblue", "bleu-ciel-clair");
	RemplacerMotEntier(texteFeuille, "mediumseagreen", QString::fromUtf8(
			"vert-oc\303\251an-moyen"));
	RemplacerMotEntier(texteFeuille, "limegreen", QString::fromUtf8(
			"citron-vert-fonc\303\251"));
	RemplacerMotEntier(texteFeuille, "lightseagreen", QString::fromUtf8(
			"vert-oc\303\251an-clair"));
	RemplacerMotEntier(texteFeuille, "darkolivegreen", "vert-olive-sombre");
	RemplacerMotEntier(texteFeuille, "navy", "bleu-marine");
	RemplacerMotEntier(texteFeuille, "darkblue", "bleu-sombre");
	RemplacerMotEntier(texteFeuille, "mediumblue", "bleu-moyen");
	RemplacerMotEntier(texteFeuille, "darkgreen", "vert-sombre");
	RemplacerMotEntier(texteFeuille, "darkcyan", "cyan-sombre");
	RemplacerMotEntier(texteFeuille, "darkturquoise", "turquoise-sombre");
	RemplacerMotEntier(texteFeuille, "lime", "citron-vert");
	RemplacerMotEntier(texteFeuille, "springgreen", "vert-printemps");
	RemplacerMotEntier(texteFeuille, "midnightblue", "bleu-nuit");
	RemplacerMotEntier(texteFeuille, "forestgreen", QString::fromUtf8(
			"vert-for\303\252t"));
	RemplacerMotEntier(texteFeuille, "seagreen", QString::fromUtf8("vert-oc\303\251an"));
	RemplacerMotEntier(texteFeuille, "royalblue", "bleu-roi");
	RemplacerMotEntier(texteFeuille, "steelblue", "bleu-acier");
	RemplacerMotEntier(texteFeuille, "mediumturquoise", "turquoise-moyen");
	RemplacerMotEntier(texteFeuille, "cadetblue", QString::fromUtf8(
			"p\303\251trole-clair"));
	RemplacerMotEntier(texteFeuille, "dimgray", "gris-rabattu");
	RemplacerMotEntier(texteFeuille, "slateblue", "bleu-ardoise");
	RemplacerMotEntier(texteFeuille, "slategray", "gris-ardoise");
	RemplacerMotEntier(texteFeuille, "lawngreen", "vert-prairie");
	RemplacerMotEntier(texteFeuille, "chartreuse", "vert-chartreuse");
	RemplacerMotEntier(texteFeuille, "aquamarine", "aigue-marine");
	RemplacerMotEntier(texteFeuille, "olive", "vert-olive");
	RemplacerMotEntier(texteFeuille, "skyblue", "bleu-ciel");
	RemplacerMotEntier(texteFeuille, "blueviolet", QString::fromUtf8(
			"parme-bleut\303\251"));
	RemplacerMotEntier(texteFeuille, "darkred", "rouge-sombre");
	RemplacerMotEntier(texteFeuille, "darkmagenta", "magenta-sombre");
	RemplacerMotEntier(texteFeuille, "lightgreen", "vert-clair");
	RemplacerMotEntier(texteFeuille, "darkviolet", "violet-sombre");
	RemplacerMotEntier(texteFeuille, "palegreen", QString::fromUtf8("vert-p\303\242le"));
	RemplacerMotEntier(texteFeuille, "darkorchid", QString::fromUtf8(
			"orchid\303\251e-sombre"));
	RemplacerMotEntier(texteFeuille, "yellowgreen", QString::fromUtf8(
			"vert-jaun\303\242tre"));
	RemplacerMotEntier(texteFeuille, "darkgray", "gris-clair");
	RemplacerMotEntier(texteFeuille, "lightblue", "bleu-clair");
	RemplacerMotEntier(texteFeuille, "greenyellow", "jaune-vert");
	RemplacerMotEntier(texteFeuille, "paleturquoise", QString::fromUtf8(
			"turquoise-p\303\242le"));
	RemplacerMotEntier(texteFeuille, "powderblue", QString::fromUtf8(
			"bleu-poudr\303\251"));
	RemplacerMotEntier(texteFeuille, "firebrick", "rouge-brique");
	RemplacerMotEntier(texteFeuille, "mediumorchid", QString::fromUtf8(
			"orchid\303\251e-moyen"));
	RemplacerMotEntier(texteFeuille, "darkkhaki", "kaki-sombre");
	RemplacerMotEntier(texteFeuille, "indianred", "rouge-indien");
	RemplacerMotEntier(texteFeuille, "tan", QString::fromUtf8("brun-tan\303\251"));
	RemplacerMotEntier(texteFeuille, "lightgrey", "gris-clair");
	RemplacerMotEntier(texteFeuille, "goldenrod", "jaune-paille");
	RemplacerMotEntier(texteFeuille, "burlywood", "bois-dur");
	RemplacerMotEntier(texteFeuille, "lightcyan", "cyan-clair");
	RemplacerMotEntier(texteFeuille, "darksalmon", "saumon-sombre");
	RemplacerMotEntier(texteFeuille, "lightcoral", "corail-clair");
	RemplacerMotEntier(texteFeuille, "aliceblue", "bleu-Alice");
	RemplacerMotEntier(texteFeuille, "sandybrown", "brun-sable");
	RemplacerMotEntier(texteFeuille, "whitesmoke", QString::fromUtf8(
			"blanc-fum\303\251e"));
	RemplacerMotEntier(texteFeuille, "mintcream", QString::fromUtf8(
			"blanc-menthol\303\251"));
	RemplacerMotEntier(texteFeuille, "ghostwhite", "blanc-spectral");
	RemplacerMotEntier(texteFeuille, "antiquewhite", "blanc-antique");
	RemplacerMotEntier(texteFeuille, "oldlace", "vieux-blanc");
	RemplacerMotEntier(texteFeuille, "deeppink", "rose-profond");
	RemplacerMotEntier(texteFeuille, "orangered", QString::fromUtf8(
			"rouge-orang\303\251"));
	RemplacerMotEntier(texteFeuille, "tomato", "rouge-tomate");
	RemplacerMotEntier(texteFeuille, "hotpink", "rose-chaud");
	RemplacerMotEntier(texteFeuille, "darkorange", "orange-sombre");
	RemplacerMotEntier(texteFeuille, "lightsalmon", "saumon-clair");
	RemplacerMotEntier(texteFeuille, "lightpink", "rose-clair");
	RemplacerMotEntier(texteFeuille, "peachpuff", QString::fromUtf8(
			"p\303\252che-pass\303\251"));
	RemplacerMotEntier(texteFeuille, "navajowhite", "blanc-navarro");
	RemplacerMotEntier(texteFeuille, "moccasin", "beige-mocassin");
	RemplacerMotEntier(texteFeuille, "mistyrose", QString::fromUtf8("rose-voil\303\251"));
	RemplacerMotEntier(texteFeuille, "blanchedalmond", "amande-blanchi");
	RemplacerMotEntier(texteFeuille, "papayawhip", QString::fromUtf8(
			"papaye-d\303\251lav\303\251"));
	RemplacerMotEntier(texteFeuille, "lavenderblush", QString::fromUtf8(
			"lavande-rouge\303\242tre"));
	RemplacerMotEntier(texteFeuille, "lemonchiffon", QString::fromUtf8(
			"jaune-chiffon\303\251"));
	RemplacerMotEntier(texteFeuille, "floralwhite", "blanc-floral");
	RemplacerMotEntier(texteFeuille, "snow", "blanc-neigeux");
	RemplacerMotEntier(texteFeuille, "lightyellow", "jaune-clair");
	RemplacerMotEntier(texteFeuille, "black", "noir");
	RemplacerMotEntier(texteFeuille, "bisque", "bisque");
	RemplacerMotEntier(texteFeuille, "honeydew", QString::fromUtf8("miell\303\251"));
	RemplacerMotEntier(texteFeuille, "orange", "orange");
	RemplacerMotEntier(texteFeuille, "coral", "corail");
	RemplacerMotEntier(texteFeuille, "wheat", QString::fromUtf8("bl\303\251"));
	RemplacerMotEntier(texteFeuille, "pink", "rose");
	RemplacerMotEntier(texteFeuille, "salmon", "saumon");
	RemplacerMotEntier(texteFeuille, "red", "rouge");
	RemplacerMotEntier(texteFeuille, "indigo", "indigo");
	RemplacerMotEntier(texteFeuille, "aqua", "eau");
	RemplacerMotEntier(texteFeuille, "cyan", "cyan");
	RemplacerMotEntier(texteFeuille, "brown", "brun");
	RemplacerMotEntier(texteFeuille, "silver", "argent");
	RemplacerMotEntier(texteFeuille, "cornflowerblue", "bleuet");
	RemplacerMotEntier(texteFeuille, "magenta", "magenta");
	RemplacerMotEntier(texteFeuille, "fuchsia", "fuchsia");
	RemplacerMotEntier(texteFeuille, "peru", QString::fromUtf8("p\303\251rou"));
	RemplacerMotEntier(texteFeuille, "chocolate", "chocolat");
	RemplacerMotEntier(texteFeuille, "maroon", "marron");
	RemplacerMotEntier(texteFeuille, "purple", "violet");
	RemplacerMotEntier(texteFeuille, "orchid", QString::fromUtf8("orchid\303\251e"));
	RemplacerMotEntier(texteFeuille, "gold", "or");
	RemplacerMotEntier(texteFeuille, "gray", "gris");
	RemplacerMotEntier(texteFeuille, "ivory", "ivoire");
	RemplacerMotEntier(texteFeuille, "seashell", "coquillage");
	RemplacerMotEntier(texteFeuille, "thistle", "chardon");
	RemplacerMotEntier(texteFeuille, "turquoise", "turquoise");
	RemplacerMotEntier(texteFeuille, "blue", "bleu");
	RemplacerMotEntier(texteFeuille, "saddlebrown", "cuir");
	RemplacerMotEntier(texteFeuille, "beige", "beige");
	RemplacerMotEntier(texteFeuille, "white", "blanc");
	RemplacerMotEntier(texteFeuille, "crimson", "pourpre");
	RemplacerMotEntier(texteFeuille, "plum", "prune");
	RemplacerMotEntier(texteFeuille, "lavender", "lavande");
	RemplacerMotEntier(texteFeuille, "green", "vert");
	RemplacerMotEntier(texteFeuille, "teal", "sarcelle");
	RemplacerMotEntier(texteFeuille, "azure", "azurin");
	RemplacerMotEntier(texteFeuille, "violet", "parme");
	RemplacerMotEntier(texteFeuille, "khaki", "kaki");
	RemplacerMotEntier(texteFeuille, "yellow", "jaune");
	return texteFeuille;
}

QString classeTypographie::FeuilleParDefaut()
{
	QStringList liste;
	liste << "corps {";
	liste << "    marge : 6px ;";
	liste << "}";
	liste << QString::fromUtf8("entr\303\251e {");
	liste << "    couleur-texte : cuir ;";
	liste << QString::fromUtf8("    taille-police : tr\303\250s-grande ;");
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << "}";
	liste << "prononciation {";
	liste << "    couleur-texte : gris-ardoise-sombre ;";
	liste << "}";
	liste << "nature {";
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << "    style-police : italique ;";
	liste << "}";
	liste << QString::fromUtf8("r\303\251sum\303\251 {");
	liste << "    couleur-fond : gris-clair ;";
	liste << "}";
	liste << QString::fromUtf8("r\303\251sum\303\251-num\303\251ro-sens {");
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << "}";
	liste << QString::fromUtf8("num\303\251ro-sens {");
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << "    couleur-fond : cuir ;";
	liste << "    couleur-texte : blanc ;";
	liste << "}";
	liste << "citation {";
	liste << "    couleur-fond : bleu-Alice ;";
	liste << "}";
	liste << QString::fromUtf8("r\303\251f\303\251rence {");
	liste << "    couleur-fond : bleu-Alice ;";
	liste << "    couleur-texte : bleu-marine ;";
	liste << "}";
	liste << QString::fromUtf8("r\303\251f\303\251rence-titre {");
	liste << "    style-police : italique ;";
	liste << "}";
	liste << "rubrique {";
	liste << "    couleur-fond : kaki ;";
	liste << "    couleur-texte : cuir ;";
	liste << "    taille-police : petite ;";
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << QString::fromUtf8("    marge-sup\303\251rieure : 15px ;");
	liste << "}";
	liste << QString::fromUtf8("suppl\303\251ment {");
	liste << "    couleur-fond : noir ;";
	liste << "    couleur-texte : blanc ;";
	liste << "    taille-police : petite ;";
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << QString::fromUtf8("    marge-sup\303\251rieure : 15px ;");
	liste << "}";
	liste << "lien {";
	liste << QString::fromUtf8("    \303\251paisseur-police : gras ;");
	liste << "    couleur-texte : vert ;";
	liste << "    effets-texte : aucun ;";
	liste << "}";
	return liste.join("\n");
}

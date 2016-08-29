#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

//MainWindow::MainWindow()
//{
//    auto *widget = new QWidget;
//    setCentralWidget(widget);
//    auto *topFiller = new QWidget;
//    topFiller->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to invoke a context menu</i>"));
//    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
//    infoLabel->setAlignment(Qt::AlignCenter);

//    auto *bottomFiller = new QWidget;
//    bottomFiller->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    auto *layout = new QVBoxLayout;
//    layout->setMargin(5);
//    layout->addWidget(topFiller);
//    layout->addWidget(infoLabel);
//    layout->addWidget(bottomFiller);
//    widget->setLayout(layout);

//    createActions();
//    createMenus();
//    auto message = tr("A context menu is available by right-clicking");
//    statusBar()->showMessage(message);
//    setWindowTitle(tr("Menus"));
//    setMinimumSize(160,160);
//    resize(480,320);
//}

//void MainWindow::createActions(){

//}

//void MainWindow::createMenus(){
//    fileMenu = menuBar()->addMenu(tr("&File"));
//}

//#ifndef QT_NO_CONTEXTMENU
//    void MainWindow::contextMenuEvent(QContextMenuEvent *event){
//        QMenu menu(this);
//        menu.exec(event->globalPos());
//    }

//#endif

MainWindow::~MainWindow()
{
    delete ui;
}

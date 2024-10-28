#include <QPushButton>
#include <QtGui/QtGui>
#include <QApplication>
#include <QAudioOutput>
#include <QtMultimedia/QMediaPlayer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget playerWindow;
    QVBoxLayout vbox(&playerWindow);
    QHBoxLayout buttons;
    auto* songSlider = new QSlider(&playerWindow);
    songSlider->setOrientation(Qt::Horizontal);
    vbox.addWidget(songSlider);

    auto* openFileButton = new QPushButton("open", &playerWindow);
    auto* playMediaButton = new QPushButton("play", &playerWindow);
    auto* pauseMediaButton = new QPushButton("pause", &playerWindow);

    buttons.addWidget(openFileButton);
    buttons.addWidget(playMediaButton);
    buttons.addWidget(pauseMediaButton);

    vbox.addLayout(&buttons);

    QString filePath;
    auto* player = new QMediaPlayer(&playerWindow);
    auto* outPut = new QAudioOutput(&playerWindow);
    player->setAudioOutput(outPut);
    bool isPlaying = false;

    QObject::connect(openFileButton, &QPushButton::clicked, [&filePath, player, &isPlaying] {
        filePath = QFileDialog::getOpenFileName(nullptr, "open mp3 song", "/opt/", "mp3 files (*.mp3)");
        player->stop();
        isPlaying = false;
        });

    QObject::connect(playMediaButton, &QPushButton::clicked, [&isPlaying, player, &filePath]() {
        if (!isPlaying) {
            player->setSource(QUrl::fromLocalFile(filePath));
            isPlaying = true;
        }
        player->play();
        });

    QObject::connect(player, &QMediaPlayer::durationChanged, [songSlider](qint64 duration) {
        songSlider->setMaximum(duration);
        songSlider->setMinimum(0);
        });

    QObject::connect(player, &QMediaPlayer::positionChanged, [songSlider](qint64 pos) {
        songSlider->setValue(pos);
        });

    QObject::connect(songSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    QObject::connect(pauseMediaButton, &QPushButton::clicked, player, &QMediaPlayer::pause);

    player->play();
    playerWindow.show();

    return QApplication::exec();
}

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QScreen>
#include <QRandomGenerator>
#include <QDateTime>
#include <QFont>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QList>
#include <QGraphicsOpacityEffect>
#include <QShortcut>
#include <QKeySequence>

class WinterGreetingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WinterGreetingWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground);

        opacityEffect = new QGraphicsOpacityEffect(this);
        opacityEffect->setOpacity(1.0);
        setGraphicsEffect(opacityEffect);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        label->setMinimumSize(200, 100);

        QFont font = label->font();
        QRandomGenerator *generator = QRandomGenerator::global();
        font.setPointSize(generator->bounded(14, 26));
        label->setFont(font);

        layout->addWidget(label);

        setWinterThemeStyle();

        setRandomGreeting();

        setupAnimation();
    }

private slots:
    void setupAnimation() {
        QSequentialAnimationGroup *sequence = new QSequentialAnimationGroup(this);
        QPropertyAnimation *fadeIn = new QPropertyAnimation(opacityEffect, "opacity");
        fadeIn->setDuration(100);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        QPauseAnimation *pause = new QPauseAnimation(this);
        pause->setDuration(10000);
        QPropertyAnimation *fadeOut = new QPropertyAnimation(opacityEffect, "opacity");
        fadeOut->setDuration(200);
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);

        sequence->addAnimation(fadeIn);
        sequence->addAnimation(pause);
        sequence->addAnimation(fadeOut);

        connect(sequence, &QSequentialAnimationGroup::finished, this, &WinterGreetingWindow::close);

        sequence->start();
    }

private:
    void setWinterThemeStyle() {
        QRandomGenerator *generator = QRandomGenerator::global();

        int red, green, blue;
        int colorType = generator->bounded(6);
        switch(colorType) {
        case 0:
            red = generator->bounded(240, 256);
            green = generator->bounded(230, 245);
            blue = generator->bounded(210, 230);
            break;
        case 1:
            red = generator->bounded(255, 256);
            green = generator->bounded(220, 240);
            blue = generator->bounded(180, 200);
            break;
        case 2:
            red = generator->bounded(210, 230);
            green = generator->bounded(180, 200);
            blue = generator->bounded(150, 170);
            break;
        case 3:
            red = generator->bounded(255, 256);
            green = generator->bounded(240, 255);
            blue = generator->bounded(200, 220);
            break;
        case 4:
            red = generator->bounded(255, 256);
            green = generator->bounded(230, 245);
            blue = generator->bounded(230, 245);
            break;
        case 5:
            red = generator->bounded(220, 240);
            green = generator->bounded(200, 220);
            blue = generator->bounded(170, 190);
            break;
        default:
            red = generator->bounded(240, 256);
            green = generator->bounded(230, 245);
            blue = generator->bounded(210, 230);
        }

        QColor bgColor(red, green, blue);

        QColor textColor;
        int lightness = bgColor.lightness();
        if (lightness > 200) {
            textColor = QColor(80, 60, 40);
        } else if (lightness > 160) {
            textColor = QColor(60, 45, 30);
        } else if (lightness > 120) {
            textColor = QColor(40, 30, 20);
        } else {
            textColor = QColor(250, 245, 240);
        }

        QString style = QString("QLabel {"
                                "color: %1;"
                                "background-color: %2;"
                                "border: 1px solid %3;"
                                "border-radius: 12px;"
                                "padding: 15px;"
                                "font-weight: 500;"
                                "}")
                            .arg(textColor.name())
                            .arg(bgColor.name())
                            .arg(textColor.darker(110).name());

        label->setStyleSheet(style);
    }

    void setRandomGreeting() {
        QStringList greetings = {
            "天冷，要暖暖的哦～",
            "冬日安好，愿你温暖。",
            "寒日里，愿你心暖。",
            "冬日快乐！",
            "雪再大，你也很暖。",
            "照顾好自己呀。",
            "愿你冬日不寒冷。",
            "暖暖的，过个冬。",
            "冬天里的小太阳～",
            "愿你被温暖包围。",
            "冬日安康。",
            "暖意满满的一天！",
            "寒冷天，暖心田。",
            "冬日顺遂。",
            "愿你三冬暖。",
            "天寒心不寒。",
            "冬日可爱，你也是～",
            "一切安好。",
            "温暖与你相伴。",
            "冬日愉快！",
            "要温暖，要快乐。",
            "爱你，老妈~",
            "暖暖手，暖暖心。",
            "冬日安，心亦安。",
            "愿你冬日可爱。",
            "冬天快乐呀！",
            "温暖常伴左右。",
            "祝君冬日安。",
            "暖暖的祝福给你～",
            "冬日安，万事顺。",
            "冬天里，祝你好。"
        };

        QRandomGenerator *generator = QRandomGenerator::global();
        int index = generator->bounded(greetings.size());
        label->setText(greetings[index]);
    }

    QLabel *label;
    QGraphicsOpacityEffect *opacityEffect;
};

class GreetingManager : public QWidget
{
    Q_OBJECT

public:
    explicit GreetingManager(QWidget *parent = nullptr) : QWidget(parent), activeWindows(0)
    {
        setWindowTitle("Winter Blessings");
        resize(1, 1);
        move(-100, -100);
        show();

        setupShortcuts();
        createTimer = new QTimer(this);
        connect(createTimer, &QTimer::timeout, this, &GreetingManager::createGreetingWindow);
        createTimer->start(200);

        batchTimer = new QTimer(this);
        connect(batchTimer, &QTimer::timeout, this, &GreetingManager::createBatchWindows);
        batchTimer->start(200);
    }

private slots:
    void createGreetingWindow() {
        if (activeWindows >= 1000) {
            return;
        }

        WinterGreetingWindow *window = new WinterGreetingWindow();
        activeWindows++;

        connect(window, &WinterGreetingWindow::destroyed, this, [this]() {
            activeWindows--;
        });

        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();

        window->resize(280, 120);

        QRandomGenerator *generator = QRandomGenerator::global();
        int x = generator->bounded(screenGeometry.width() - window->width());
        int y = generator->bounded(screenGeometry.height() - window->height());

        window->move(x, y);
        window->show();
    }

    void createBatchWindows() {
        QRandomGenerator *generator = QRandomGenerator::global();
        int batchSize = generator->bounded(3, 7);

        for (int i = 0; i < batchSize; i++) {
            QTimer::singleShot(i * 30, this, &GreetingManager::createGreetingWindow);
        }
    }

    void quitApplication() {
        QApplication::quit();
    }

private:
    void setupShortcuts() {
        // 创建 Ctrl+Q 快捷键
        QShortcut *quitShortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
        connect(quitShortcut, &QShortcut::activated, this, &GreetingManager::quitApplication);

        // 添加 Esc 键
        QShortcut *escShortcut = new QShortcut(QKeySequence("Esc"), this);
        connect(escShortcut, &QShortcut::activated, this, &GreetingManager::quitApplication);

        // 添加 Alt+F4 快捷键
        QShortcut *altF4Shortcut = new QShortcut(QKeySequence("Alt+F4"), this);
        connect(altF4Shortcut, &QShortcut::activated, this, &GreetingManager::quitApplication);
    }

    QTimer *createTimer;
    QTimer *batchTimer;
    int activeWindows;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setAttribute(Qt::AA_UseSoftwareOpenGL);

    GreetingManager manager;

    return app.exec();
}

#include "main.moc"

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
        fadeIn->setDuration(300);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        QPauseAnimation *pause = new QPauseAnimation(this);
        pause->setDuration(10000);
        QPropertyAnimation *fadeOut = new QPropertyAnimation(opacityEffect, "opacity");
        fadeOut->setDuration(1000);
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
            "寒冬已至，愿温暖常伴你左右！",
            "雪花飘落，愿你的生活如诗如画！",
            "冬日暖阳，愿你心中充满希望！",
            "寒风凛冽，愿你的心依然温暖！",
            "瑞雪兆丰年，愿你收获满满！",
            "冬日安康，愿健康与你同行！",
            "白雪皑皑，愿你的世界纯净美好！",
            "冬日漫长，愿快乐永不缺席！",
            "天寒地冻，愿你的心依然火热！",
            "冬日静好，愿岁月温柔待你！",
            "寒风刺骨，愿温暖永驻心间！",
            "冬日暖炉，愿幸福围绕着你！",
            "雪花飞舞，愿你的梦想绽放！",
            "冬日祝福，愿快乐与你同在！",
            "冰天雪地，愿你的热情不减！",
            "冬日阳光，愿你的生活灿烂！",
            "寒风呼啸，愿你的心依然宁静！",
            "冬日问候，愿平安伴你左右！",
            "白雪纷飞，愿你的世界美丽！",
            "冬日温暖，愿幸福永驻心田！",
            "冬日寒梅，愿你坚韧不拔！",
            "冬日炉火，愿你温馨相伴！",
            "冬日雪花，愿你纯洁无瑕！",
            "冬日星空，愿你梦想璀璨！",
            "冬日晨曦，愿你充满活力！",
            "冬日黄昏，愿你宁静安详！",
            "冬日思念，愿你有人牵挂！",
            "冬日团聚，愿你家庭和睦！",
            "冬日美食，愿你胃口大开！",
            "冬日读书，愿你知识增长！"
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
        createTimer->start(100);

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

        window->resize(320, 140);

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

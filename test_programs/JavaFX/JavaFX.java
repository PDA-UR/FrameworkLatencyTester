import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Rectangle2D;
import javafx.scene.Scene;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Screen;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

import java.io.IOException;
import java.util.Random;

public class JavaFX extends Application {
	static int rect_count = 0;

    private int WIDTH = 1920;
    private int HEIGHT = 1080;

	Pane pane;
	Scene scene;
	Rectangle whiteRect;
	Random r = new Random();

	@Override
    public void start(Stage stage) throws IOException {
		this.rect_count = rect_count;
        stage.setTitle("JavaFX Test Application");
		//stage.initStyle(StageStyle.UNDECORATED);
		stage.setFullScreen(true);
		stage.setX(0);
		stage.setY(0);

		pane = new Pane();
		scene = new Scene(pane, WIDTH, HEIGHT);

		BackgroundFill backgroundFill = new BackgroundFill(Color.BLACK, CornerRadii.EMPTY, Insets.EMPTY);
		Background background = new Background(backgroundFill);
		pane.setBackground(background);

		pane.setOnMousePressed(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                mousePressed();
            }
        });

		pane.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                mouseReleased();
            }
        });

		stage.setScene(scene);
		stage.show();
    }

    public void mousePressed() {
		whiteRect = new Rectangle(0, 0, WIDTH, HEIGHT);
		whiteRect.setFill(new Color(1, 1, 1, 1));
		pane.getChildren().add(whiteRect);
    }

    public void mouseReleased() {
		pane.getChildren().remove(whiteRect);
    }

    public static void main(String[] args) {
		launch();
    }
}

#include "score.h"

void handle_score(int score)
{
    HighScore *scores = get_scores();
    for (int i = 0; i < NR_OF_SCORES; ++i) {
        if (score > scores[i].score) {
            for (int j = NR_OF_SCORES - 1; j > i; --j) {
                scores[j] = scores[j - 1];
            }
            scores[i].score = score;
            char *name = get_name();
            strcpy(scores[i].name, name);
            free(name);
            write_scores(scores);
            break;
        }
    }
    free(scores);
}


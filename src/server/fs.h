#pragma once

NodeMatcherArray *fs_new_file_path_matchers(char *files[], int count);

NodeConditionFunc fs_new_filename_condition(char *filename);

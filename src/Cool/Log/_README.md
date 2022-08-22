## `OptionalErrorMessage`

Our strategy for handling user-facing errors: we let the low level return an `OptionalErrorMessage`; it will bubble up, maybe intermediate layers can tweak the error message, and at some high-level we will decide what to do with that error: how the program should continue, and how to display that message: is it a warning? An error? What is the category?

`OptionalErrorMessage` is a `nodiscard` type so you cannot forget to handle the error.

This is much better than the previous strategy we used, where it was the low-level (which, as always, detects the error) that was also repsonsible for logging it to the user: it lacked information, resulting in poor error messages. And worse, in some cases we just wanted to handle the error internally and not show it to the user, which was no possible with this strategy (unless we added booleans everywhere and passed them down the chain, which was ugly).

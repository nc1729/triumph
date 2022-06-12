;; tas-mode.el
;; Emacs script for edit mode for .tas files.
;; Based heavily on https://www.emacswiki.org/emacs/ModeTutorial

;;; Code:
(defvar tas-mode-hook nil)
(defvar tas-mode-map
  (let ((tas-mode-map (make-keymap)))
    (define-key tas-mode-map "\C-j" 'newline-and-indent)
    tas-mode-map)
  "Keymap for TAS major mode")

(setq tas-tab-width 4)

(add-to-list 'auto-mode-alist '("\\.tas\\'" . tas-mode))

(defconst tas-font-lock-keywords-1
  (list   '("\\<\\(A\\(?:D[CD]\\|ND\\)\\|BANK\\|C\\(?:MP\\|PZ\\)\\|DEC\\|FLIP\\|GANK\\|HALT\\|INC\\|JP\\(?:N[NPZ]\\|[NPSZ]\\)?\\|LOAD\\|NO[PT]\\|OR\\|P\\(?:EEK\\|JP\\|OP\\|USH\\)\\|S\\(?:AVE\\|BB\\|ET\\|H\\|TAR\\|UB\\|WAP\\)\\|ZERO\\)\\>" . font-lock-builtin-face)
   '("\\('\\w*'\\)" . font-lock-variable-name-face))
  "Minimal highlighting expressions for TAS mode.")

(defvar tas-font-lock-keywords tas-font-lock-keywords-1
  "Default highlighting expressions for TAS mode.")

(defun tas-indent-line ()
  "Indent current line as TAS code."
  (interactive)
  (beginning-of-line)
  (if (bobp)
	  (indent-line-to 0)		   ; First line is always non-indented
	(let ((not-indented t) cur-indent)
	  (if (looking-at "[ \t]*}") ; If the line we are looking at is the end of a block, then decrease the indentation
		  (progn
			(save-excursion
			  (forward-line -1)
			  (setq cur-indent (- (current-indentation) tas-tab-width)))
			(if (< cur-indent 0) ; We can't indent past the left margin
				(setq cur-indent 0)))
		(save-excursion
		  (while not-indented ; Iterate backwards until we find an indentation hint
			(forward-line -1)
			(if (looking-at "[ \t]*}") ; This hint indicates that we need to indent at the level of the END_ token
			    (progn
			      (setq cur-indent (current-indentation))
				  (setq not-indented nil))
			  (if (looking-at "[ \t]*{") ; This hint indicates that we need to indent an extra level
				  (progn
					(setq cur-indent (+ (current-indentation) tas-tab-width)) ; Do the actual indenting
					(setq not-indented nil))
				(if (bobp)
					(setq not-indented nil)))))))
	  (if cur-indent
		  (indent-line-to cur-indent)
		(indent-line-to 0))))) ; If we didn't see an indentation hint, then allow no indentation

(defvar tas-mode-syntax-table
  (let ((tas-mode-syntax-table (make-syntax-table)))
	
    ; This is added so entity names with underscores can be more easily parsed
	(modify-syntax-entry ?_ "w" tas-mode-syntax-table)
	
	; Comment styles are same as C++
	(modify-syntax-entry ?/ ". 124b" tas-mode-syntax-table)
	(modify-syntax-entry ?* ". 23" tas-mode-syntax-table)
	(modify-syntax-entry ?\n "> b" tas-mode-syntax-table)
	tas-mode-syntax-table)
  "Syntax table for tas-mode")
  
(defun tas-mode ()
  (interactive)
  (kill-all-local-variables)
  (use-local-map tas-mode-map)
  (set-syntax-table tas-mode-syntax-table)
  ;; Set up font-lock
  (set (make-local-variable 'font-lock-defaults) '(tas-font-lock-keywords))
  ;; Register our indentation function
  (set (make-local-variable 'indent-line-function) 'tas-indent-line)  
  (setq major-mode 'tas-mode)
  (setq mode-name "TAS")
  (run-hooks 'tas-mode-hook))

(provide 'tas-mode)

;;; tas-mode.el ends here

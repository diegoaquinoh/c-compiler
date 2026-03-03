// Generated from ifcc.g4 by ANTLR 4.13.2
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class ifccLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, RETURN=8, CONST=9, 
		COMMENT=10, DIRECTIVE=11, WS=12;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "RETURN", "CONST", 
			"COMMENT", "DIRECTIVE", "WS"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'int'", "'main'", "'('", "')'", "'{'", "'}'", "';'", "'return'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, "RETURN", "CONST", "COMMENT", 
			"DIRECTIVE", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public ifccLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "ifcc.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\u0004\u0000\fU\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002\u0001"+
		"\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004"+
		"\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007"+
		"\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b"+
		"\u0007\u000b\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0001"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002"+
		"\u0001\u0003\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0005\u0001\u0005"+
		"\u0001\u0006\u0001\u0006\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\u0007\u0001\b\u0004\b5\b\b\u000b\b\f\b"+
		"6\u0001\t\u0001\t\u0001\t\u0001\t\u0005\t=\b\t\n\t\f\t@\t\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0001\n\u0001\n\u0005\nI\b\n\n\n\f\nL\t\n\u0001"+
		"\n\u0001\n\u0001\n\u0001\n\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0002>J\u0000\f\u0001\u0001\u0003\u0002\u0005\u0003\u0007\u0004\t\u0005"+
		"\u000b\u0006\r\u0007\u000f\b\u0011\t\u0013\n\u0015\u000b\u0017\f\u0001"+
		"\u0000\u0002\u0001\u000009\u0003\u0000\t\n\r\r  W\u0000\u0001\u0001\u0000"+
		"\u0000\u0000\u0000\u0003\u0001\u0000\u0000\u0000\u0000\u0005\u0001\u0000"+
		"\u0000\u0000\u0000\u0007\u0001\u0000\u0000\u0000\u0000\t\u0001\u0000\u0000"+
		"\u0000\u0000\u000b\u0001\u0000\u0000\u0000\u0000\r\u0001\u0000\u0000\u0000"+
		"\u0000\u000f\u0001\u0000\u0000\u0000\u0000\u0011\u0001\u0000\u0000\u0000"+
		"\u0000\u0013\u0001\u0000\u0000\u0000\u0000\u0015\u0001\u0000\u0000\u0000"+
		"\u0000\u0017\u0001\u0000\u0000\u0000\u0001\u0019\u0001\u0000\u0000\u0000"+
		"\u0003\u001d\u0001\u0000\u0000\u0000\u0005\"\u0001\u0000\u0000\u0000\u0007"+
		"$\u0001\u0000\u0000\u0000\t&\u0001\u0000\u0000\u0000\u000b(\u0001\u0000"+
		"\u0000\u0000\r*\u0001\u0000\u0000\u0000\u000f,\u0001\u0000\u0000\u0000"+
		"\u00114\u0001\u0000\u0000\u0000\u00138\u0001\u0000\u0000\u0000\u0015F"+
		"\u0001\u0000\u0000\u0000\u0017Q\u0001\u0000\u0000\u0000\u0019\u001a\u0005"+
		"i\u0000\u0000\u001a\u001b\u0005n\u0000\u0000\u001b\u001c\u0005t\u0000"+
		"\u0000\u001c\u0002\u0001\u0000\u0000\u0000\u001d\u001e\u0005m\u0000\u0000"+
		"\u001e\u001f\u0005a\u0000\u0000\u001f \u0005i\u0000\u0000 !\u0005n\u0000"+
		"\u0000!\u0004\u0001\u0000\u0000\u0000\"#\u0005(\u0000\u0000#\u0006\u0001"+
		"\u0000\u0000\u0000$%\u0005)\u0000\u0000%\b\u0001\u0000\u0000\u0000&\'"+
		"\u0005{\u0000\u0000\'\n\u0001\u0000\u0000\u0000()\u0005}\u0000\u0000)"+
		"\f\u0001\u0000\u0000\u0000*+\u0005;\u0000\u0000+\u000e\u0001\u0000\u0000"+
		"\u0000,-\u0005r\u0000\u0000-.\u0005e\u0000\u0000./\u0005t\u0000\u0000"+
		"/0\u0005u\u0000\u000001\u0005r\u0000\u000012\u0005n\u0000\u00002\u0010"+
		"\u0001\u0000\u0000\u000035\u0007\u0000\u0000\u000043\u0001\u0000\u0000"+
		"\u000056\u0001\u0000\u0000\u000064\u0001\u0000\u0000\u000067\u0001\u0000"+
		"\u0000\u00007\u0012\u0001\u0000\u0000\u000089\u0005/\u0000\u00009:\u0005"+
		"*\u0000\u0000:>\u0001\u0000\u0000\u0000;=\t\u0000\u0000\u0000<;\u0001"+
		"\u0000\u0000\u0000=@\u0001\u0000\u0000\u0000>?\u0001\u0000\u0000\u0000"+
		"><\u0001\u0000\u0000\u0000?A\u0001\u0000\u0000\u0000@>\u0001\u0000\u0000"+
		"\u0000AB\u0005*\u0000\u0000BC\u0005/\u0000\u0000CD\u0001\u0000\u0000\u0000"+
		"DE\u0006\t\u0000\u0000E\u0014\u0001\u0000\u0000\u0000FJ\u0005#\u0000\u0000"+
		"GI\t\u0000\u0000\u0000HG\u0001\u0000\u0000\u0000IL\u0001\u0000\u0000\u0000"+
		"JK\u0001\u0000\u0000\u0000JH\u0001\u0000\u0000\u0000KM\u0001\u0000\u0000"+
		"\u0000LJ\u0001\u0000\u0000\u0000MN\u0005\n\u0000\u0000NO\u0001\u0000\u0000"+
		"\u0000OP\u0006\n\u0000\u0000P\u0016\u0001\u0000\u0000\u0000QR\u0007\u0001"+
		"\u0000\u0000RS\u0001\u0000\u0000\u0000ST\u0006\u000b\u0001\u0000T\u0018"+
		"\u0001\u0000\u0000\u0000\u0004\u00006>J\u0002\u0006\u0000\u0000\u0000"+
		"\u0001\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}
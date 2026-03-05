// Generated from c:/Louistravail/4IF/GL/Comp/squelette-pld-comp/squelette-pld-comp/compiler/ifcc.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class ifccParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, RETURN=9, 
		CONST=10, COMMENT=11, DIRECTIVE=12, WS=13, VARNAME=14;
	public static final int
		RULE_axiom = 0, RULE_prog = 1, RULE_statement = 2, RULE_return_stmt = 3, 
		RULE_init_stmt = 4;
	private static String[] makeRuleNames() {
		return new String[] {
			"axiom", "prog", "statement", "return_stmt", "init_stmt"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'int'", "'main'", "'('", "')'", "'{'", "'}'", "';'", "'='", "'return'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, "RETURN", "CONST", 
			"COMMENT", "DIRECTIVE", "WS", "VARNAME"
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

	@Override
	public String getGrammarFileName() { return "ifcc.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public ifccParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AxiomContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TerminalNode EOF() { return getToken(ifccParser.EOF, 0); }
		public AxiomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_axiom; }
	}

	public final AxiomContext axiom() throws RecognitionException {
		AxiomContext _localctx = new AxiomContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_axiom);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(10);
			prog();
			setState(11);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ProgContext extends ParserRuleContext {
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_prog);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(13);
			match(T__0);
			setState(14);
			match(T__1);
			setState(15);
			match(T__2);
			setState(16);
			match(T__3);
			setState(17);
			match(T__4);
			setState(19); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(18);
				statement();
				}
				}
				setState(21); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==T__0 || _la==RETURN );
			setState(23);
			match(T__5);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StatementContext extends ParserRuleContext {
		public Return_stmtContext return_stmt() {
			return getRuleContext(Return_stmtContext.class,0);
		}
		public Init_stmtContext init_stmt() {
			return getRuleContext(Init_stmtContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_statement);
		try {
			setState(27);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case RETURN:
				enterOuterAlt(_localctx, 1);
				{
				setState(25);
				return_stmt();
				}
				break;
			case T__0:
				enterOuterAlt(_localctx, 2);
				{
				setState(26);
				init_stmt();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Return_stmtContext extends ParserRuleContext {
		public Return_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_return_stmt; }
	 
		public Return_stmtContext() { }
		public void copyFrom(Return_stmtContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Return_const_stmtContext extends Return_stmtContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Return_const_stmtContext(Return_stmtContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Return_var_stmtContext extends Return_stmtContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public TerminalNode VARNAME() { return getToken(ifccParser.VARNAME, 0); }
		public Return_var_stmtContext(Return_stmtContext ctx) { copyFrom(ctx); }
	}

	public final Return_stmtContext return_stmt() throws RecognitionException {
		Return_stmtContext _localctx = new Return_stmtContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_return_stmt);
		try {
			setState(35);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,2,_ctx) ) {
			case 1:
				_localctx = new Return_const_stmtContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(29);
				match(RETURN);
				setState(30);
				match(CONST);
				setState(31);
				match(T__6);
				}
				break;
			case 2:
				_localctx = new Return_var_stmtContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(32);
				match(RETURN);
				setState(33);
				match(VARNAME);
				setState(34);
				match(T__6);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Init_stmtContext extends ParserRuleContext {
		public Init_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_init_stmt; }
	 
		public Init_stmtContext() { }
		public void copyFrom(Init_stmtContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Init_stmt_varnameContext extends Init_stmtContext {
		public List<TerminalNode> VARNAME() { return getTokens(ifccParser.VARNAME); }
		public TerminalNode VARNAME(int i) {
			return getToken(ifccParser.VARNAME, i);
		}
		public Init_stmt_varnameContext(Init_stmtContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Init_stmt_constContext extends Init_stmtContext {
		public TerminalNode VARNAME() { return getToken(ifccParser.VARNAME, 0); }
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Init_stmt_constContext(Init_stmtContext ctx) { copyFrom(ctx); }
	}

	public final Init_stmtContext init_stmt() throws RecognitionException {
		Init_stmtContext _localctx = new Init_stmtContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_init_stmt);
		try {
			setState(47);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,3,_ctx) ) {
			case 1:
				_localctx = new Init_stmt_constContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(37);
				match(T__0);
				setState(38);
				match(VARNAME);
				setState(39);
				match(T__7);
				setState(40);
				match(CONST);
				setState(41);
				match(T__6);
				}
				break;
			case 2:
				_localctx = new Init_stmt_varnameContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(42);
				match(T__0);
				setState(43);
				match(VARNAME);
				setState(44);
				match(T__7);
				setState(45);
				match(VARNAME);
				setState(46);
				match(T__6);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001\u000e2\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0001"+
		"\u0000\u0001\u0000\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0001\u0001\u0001\u0004\u0001\u0014\b\u0001\u000b\u0001\f"+
		"\u0001\u0015\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002\u0003\u0002"+
		"\u001c\b\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0001\u0003\u0003\u0003$\b\u0003\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0003\u00040\b\u0004\u0001\u0004\u0000\u0000\u0005\u0000"+
		"\u0002\u0004\u0006\b\u0000\u00000\u0000\n\u0001\u0000\u0000\u0000\u0002"+
		"\r\u0001\u0000\u0000\u0000\u0004\u001b\u0001\u0000\u0000\u0000\u0006#"+
		"\u0001\u0000\u0000\u0000\b/\u0001\u0000\u0000\u0000\n\u000b\u0003\u0002"+
		"\u0001\u0000\u000b\f\u0005\u0000\u0000\u0001\f\u0001\u0001\u0000\u0000"+
		"\u0000\r\u000e\u0005\u0001\u0000\u0000\u000e\u000f\u0005\u0002\u0000\u0000"+
		"\u000f\u0010\u0005\u0003\u0000\u0000\u0010\u0011\u0005\u0004\u0000\u0000"+
		"\u0011\u0013\u0005\u0005\u0000\u0000\u0012\u0014\u0003\u0004\u0002\u0000"+
		"\u0013\u0012\u0001\u0000\u0000\u0000\u0014\u0015\u0001\u0000\u0000\u0000"+
		"\u0015\u0013\u0001\u0000\u0000\u0000\u0015\u0016\u0001\u0000\u0000\u0000"+
		"\u0016\u0017\u0001\u0000\u0000\u0000\u0017\u0018\u0005\u0006\u0000\u0000"+
		"\u0018\u0003\u0001\u0000\u0000\u0000\u0019\u001c\u0003\u0006\u0003\u0000"+
		"\u001a\u001c\u0003\b\u0004\u0000\u001b\u0019\u0001\u0000\u0000\u0000\u001b"+
		"\u001a\u0001\u0000\u0000\u0000\u001c\u0005\u0001\u0000\u0000\u0000\u001d"+
		"\u001e\u0005\t\u0000\u0000\u001e\u001f\u0005\n\u0000\u0000\u001f$\u0005"+
		"\u0007\u0000\u0000 !\u0005\t\u0000\u0000!\"\u0005\u000e\u0000\u0000\""+
		"$\u0005\u0007\u0000\u0000#\u001d\u0001\u0000\u0000\u0000# \u0001\u0000"+
		"\u0000\u0000$\u0007\u0001\u0000\u0000\u0000%&\u0005\u0001\u0000\u0000"+
		"&\'\u0005\u000e\u0000\u0000\'(\u0005\b\u0000\u0000()\u0005\n\u0000\u0000"+
		")0\u0005\u0007\u0000\u0000*+\u0005\u0001\u0000\u0000+,\u0005\u000e\u0000"+
		"\u0000,-\u0005\b\u0000\u0000-.\u0005\u000e\u0000\u0000.0\u0005\u0007\u0000"+
		"\u0000/%\u0001\u0000\u0000\u0000/*\u0001\u0000\u0000\u00000\t\u0001\u0000"+
		"\u0000\u0000\u0004\u0015\u001b#/";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}